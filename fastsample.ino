#undef HID_ENABLED

// Arduino Due ADC->DMA->USB 1MSPS
// by stimmer
// from http://forum.arduino.cc/index.php?topic=137635.msg1136315#msg1136315
// Input: Analog in A0
// Output: Raw stream of uint16_t in range 0-4095 on Native USB Serial/ACM

// on linux, to stop the OS cooking your data: 
// stty -F /dev/ttyACM0 raw -iexten -echo -echoe -echok -echoctl -echoke -onlcr

volatile int bufn,obufn;
uint16_t buf[4][256];   // 4 buffers of 256 readings
//int st;

void ADC_Handler(){     // move DMA pointers to next buffer
  int f=ADC->ADC_ISR; //   read the interrupt status register
  if (f&(1<<27)){ /// check the bit "endrx"  in the status register
   /// set up the "next pointer register" 
   bufn=(bufn+1)&3;
   ADC->ADC_RNPR=(uint32_t)buf[(bufn+1)&3];
   // set up the "next counter"
   ADC->ADC_RNCR=256;
  } 
}

void setup(){
  SerialUSB.begin(0);
  while(!SerialUSB);
  pmc_enable_periph_clk(ID_ADC); //power management controller told to turn on adc
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  ADC->ADC_MR |=0x80; // free running

  ADC->ADC_CHER=0x80; 

  NVIC_EnableIRQ(ADC_IRQn); // interrupt controller set to enable adc.
  ADC->ADC_IDR=~(1<<27); // interrupt disable register, disables all interrupts but ENDRX
  ADC->ADC_IER=1<<27; // interrupt enable register, enables only ENDRX
  ADC->ADC_RPR=(uint32_t)buf[0];   // DMA buffer
  ADC->ADC_RCR=256;
  ADC->ADC_RNPR=(uint32_t)buf[1]; // next DMA buffer
  ADC->ADC_RNCR=256;
  bufn=obufn=1;
  ADC->ADC_PTCR=1;// transfer control register for the DMA is set to enable receiver channel requests
  ADC->ADC_CR=2;
}

void loop(){
  //st = micros();
  while(obufn==bufn); // wait for buffer to be full
  //SerialUSB.println(micros()-st);
  SerialUSB.write((uint8_t *)buf[obufn],512); // send it - 512 bytes = 256 uint16_t
  obufn=(obufn+1)&3;    
}

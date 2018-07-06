#!/usr/bin/python

import serial
import time
import numpy as np
import os

#port = '/dev/tty.usbmodem1411'
port = 'COM10'
ser = serial.Serial(port,timeout=5)
# We collect N*chunkSize datapoints
N = 1
chunkSize = 100000
timestr = time.strftime("%Y%m%d-%H%M%S")+".csv"
# Should be comment later
try:
    os.remove('test.csv')
except IOError:
    pass

ser.flushInput()
for i in range(0,N):
    # read one full chunk from the serial port
    data = ser.read(chunkSize*2)
    # convert data to 16bit int numpy array
    data = np.frombuffer(data, dtype=np.uint16)
    #print(len(data))
    print(N-i)
    #with open(timestr,'a') as f:
    with open('test.csv','a') as f:
        np.savetxt(f,data,delimiter=",")
    
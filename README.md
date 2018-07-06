# fastArduinoDueADC

This is a project that realizes a fast one-channel ADC in Arduino DUE. It can be used to sample the analog audio signal from chip FC-109 (http://electropit.com/index.php/2015/12/20/fc-109-max9812-microphone-amplifier-module/). 

## Credits to
This project is based on :
1. pklaus @ https://gist.github.com/pklaus/5921022
2. comments from borisbarbour

## Sampling Frequency of ADC in Arduino DUE (fastsample.ino):
Baed on my measurement, it can achieve a sampling frequency of fs = 256(points)/370(uS)*1e6=691891.89Hz.

## From Arduino to PC (read_arduino_due.py)
The data points will be saved to a file named "test.csv". You can also use the time as the file name.
The total number of the points are decided by (N * chunkSize)

Feel free to post your suggestions here.

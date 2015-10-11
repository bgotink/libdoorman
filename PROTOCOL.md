# Doorman Protocol

The doorman portable chime sends/receives a signal on the 433MHz band.
The signal consists of 38 bits, sent over 80 times with ±30 0-bits pause in between.

The signal looks like this:

    10X10X10X10X10Y10Y10Y10Y10Y10Y10Y10Y10

with `XXXX` the channel number and `YYYYYYYY` the song number.

## Channels

The doorman has 16 channels, which are represented by 4 bits:

```
 1: 1111
 2: 0011
 3: 1011
 4: 0000
 5: 1000
 6: 0100
 7: 1100
 8: 0101
 9: 1101
10: 0001
11: 1001
12: 0010
13: 1010
14: 0110
15: 1110
16: 0111
```

## Songs

The doorman uses 8 bits to send which song to play. We don't know yet which numbers
represent actual songs that can be played.
The following three songs are sent by our doorman 46011SL transmitter:

```
1: 11110111
2: 11111011
3: 11111101
```

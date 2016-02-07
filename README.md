# QuarkTx, up to 17 Channels !!
Radio Control system based on Arduino Nano ATmega 328  16 Mhz or Teensy 3.2 96 Mhz, with a simple aim: Provide powerfull embedded functions in the smalest piece of hardware.

## Doc
Radio [user guide](https://github.com/hexabyte23/QuarkTx/blob/master/Doc/radio.md)


Gui [user guide](https://github.com/hexabyte23/QuarkTx/blob/master/Doc/gui.md)

## Gui dir
Qt client source code for WIN 10, OSX, Android & IOS

## Radio dir
Arduino Nano ATmega328/Teensy 3.2 source code

## RF Module
We just test with a Jeti TU2 2.4 GHz RF module. Smart interrupt code is able to generate up to 17 different PPM channels !!!

## RCL: a new language
We introduce RCL, a real time Radio Control Language, that include reverse, dual rate, exponetial and mixer functions per channel.

The average update rate of channels is more than 300 Hz for a 6 channels config with simple RCL commands.
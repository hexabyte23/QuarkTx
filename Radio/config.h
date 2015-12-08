#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "0.0.3" 
#define SERIAL_SPEED 9600

#define MAX_SERIAL_INPUT_BUFFER 10
#define MAX_MODEL 8
#define MAX_ADC_CHANNEL 8
#define MAX_PPM_CHANNEL 6
#define MAX_MIXER 8

#define PPM_MIN_VALUE   1000
#define PPM_MID_VALUE   1500
#define PPM_MAX_VALUE   2000
#define PPM_POLARITY    1       //set polarity of the pulses: 1 is positive, 0 is negative
#define PPM_FRAME_LEN   22500   //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PULSE_LEN   300     //set the pulse length

#define PPM_PIN 10
#define LED_PIN 13

//#define GET_ADC_BY_IRQ

#endif

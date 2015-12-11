#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "0.0.6" 
#define SERIAL_SPEED 9600

#define MAX_MODEL 4
#define MAX_SERIAL_INPUT_BUFFER 10
#define MAX_ADC_INPUT_CHANNEL 4       // Gimbal + extra pot
#define MAX_DIG_INPUT_CHANNEL 2       // Mecanical switches
#define MAX_PPM_OUTPUT_CHANNEL 4
#define MAX_MIXER 8

#define PPM_MIN_VALUE   1000
#define PPM_MAX_VALUE   2000
#define PPM_INVERT      false   // set polarity of the pulses: 1 is positive, 0 is negative
#define PPM_FRAME_LEN   22500   // set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PULSE_LEN   300     // set the pulse length
#define PPM_DEBUG       false   // /PPM signal speed / 256
#define PPM_PULSE_LEN   448     // in microseconds
#define PPM_PAUSE_LEN   10448   // in microseconds

#define TX_MODE         1       // mode 1 : left stick operates elevator & rudder, right stick operates throttle & ailerons
                                // mode 2 : left stick operates throttle & rudder, right stick operates elevator & ailerons
                                // mode 3 : left stick operates elevator & ailerons, right stick operates throttle & rudder
                                // mode 4 : left stick operates throttle & ailerons, right stick operates elevator & rudder

#define PPM_PIN 10
#define LED_PIN 13

//#define GET_ADC_BY_IRQ

#endif

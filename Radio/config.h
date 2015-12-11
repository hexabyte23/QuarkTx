#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "0.0.7" 
#define SERIAL_SPEED 9600

#define DISPLAY_BASE  DEC             // DEC or HEX

#define MAX_MODEL 4
#define MAX_SERIAL_INPUT_BUFFER 10
#define MAX_ADC_INPUT_CHANNEL 4       // Gimbal + extra pot
#define MAX_DIG_INPUT_CHANNEL 2       // Mecanical switches
#define MAX_PPM_OUTPUT_CHANNEL 4
#define MAX_MIXER 8

#define PPM_MIN_VALUE   1000
#define PPM_MAX_VALUE   2000
#define PPM_INVERT      false   // false positive signal, true negative signal
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

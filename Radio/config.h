#ifndef CONFIG_H
#define CONFIG_H


//#define TERRATOP                // Define the hardware you want to test

#ifdef TERRATOP

#define MAX_ADC_INPUT_CHANNEL   4       // Gimballs
#define MAX_DIG_INPUT_CHANNEL   3       // Mechanical switches
#define MAX_PPM_OUTPUT_CHANNEL  6
#define MAX_MIXER               8
#define PPM_SHAPE_SIGNAL        1       // 1 is positive, 0 is negative
#define PPM_PIN                 10
#define SWITCH1_PIN             2
#define SWITCH2_PIN             3
#define SWITCH3_PIN             4
#define LED_PIN                 6
#define BT_RX_PIN               8
#define BT_TX_PIN               9
#define PPM_PULSE_LEN           300     // in microseconds
#define PPM_FRAME_LEN           22500   // in microseconds

#else

#define MAX_ADC_INPUT_CHANNEL   4       // Gimballs
#define MAX_DIG_INPUT_CHANNEL   2       // Mechanical switches
#define MAX_PPM_OUTPUT_CHANNEL  6
#define MAX_MIXER               8
#define PPM_SHAPE_SIGNAL        1       // 1 is positive, 0 is negative
#define PPM_PIN                 10
#define SWITCH1_PIN             2
#define SWITCH2_PIN             3
#define LED_PIN                 13
#define BT_RX_PIN               8
#define BT_TX_PIN               9
#define PPM_PULSE_LEN           300     // in microseconds
#define PPM_FRAME_LEN           22500   // in microseconds
#endif


// General definitions

#define VERSION                 "0.0.12" 
#define SERIAL_SPEED            9600

#define DISPLAY_BASE            DEC     // DEC or HEX

#define MAX_MODEL               2
#define MAX_SERIAL_INPUT_BUFFER 10
#define MAX_INPUT_CHANNEL       MAX_ADC_INPUT_CHANNEL+MAX_DIG_INPUT_CHANNEL

#define ADC_MIN_VALUE           0
#define ADC_MAX_VALUE           1023

#define PPM_MIN_VALUE           1000
#define PPM_MAX_VALUE           2000

#define TX_MODE                 1       // mode 1 : left stick operates elevator & rudder, right stick operates throttle & ailerons
                                        // mode 2 : left stick operates throttle & rudder, right stick operates elevator & ailerons
                                        // mode 3 : left stick operates elevator & ailerons, right stick operates throttle & rudder
                                        // mode 4 : left stick operates throttle & ailerons, right stick operates elevator & rudder

#define LED_BLINK_PERIOD        500    // millisec

#endif

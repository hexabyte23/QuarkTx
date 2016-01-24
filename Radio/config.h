/*
Config.h - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef CONFIG_H
#define CONFIG_H

// General definitions

#define QUARKTX_VERSION         "0.1.0"
#define QUARKTX_SERIAL_SPEED    9600

#define MAX_MODEL               2
#define MAX_SERIAL_INPUT_BUFFER 40      // warning: dont set too small value for RCL commands

#define ADC_MIN_VALUE           0
#define ADC_MAX_VALUE           1023

#define PPM_MIN_VALUE           500
#define PPM_MAX_VALUE           1500

#define TRIM_MIN_VALUE          -100
#define TRIM_MAX_VALUE          100

#define TX_MODE                 1       // mode 1 : left stick operates elevator & rudder, right stick operates throttle & ailerons
                                        // mode 2 : left stick operates throttle & rudder, right stick operates elevator & ailerons
                                        // mode 3 : left stick operates elevator & ailerons, right stick operates throttle & rudder
                                        // mode 4 : left stick operates throttle & ailerons, right stick operates elevator & rudder

#define LED_BLINK_PERIOD        500    // millisec

#define SERIAL_END_CMD_CHAR     '\r'


//
//
//  Put here after all definitions relative to your configuration
//
//

#define TERRATOP
//#define BLUETOOTH             
#define JETI_TU2_MODULE



#ifdef TERRATOP

#define MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define MAX_DIG_INPUT_CHANNEL   3       // Mechanical switches
#define MAX_PPM_OUTPUT_CHANNEL  6       // 17 channels max with Jeti TU2 module

#define PPM_PIN                 10

#define SWITCH1_PIN             2
#define SWITCH2_PIN             3
#define SWITCH3_PIN             4

#define LED_PIN                 13

#define BT_RX_PIN               9
#define BT_TX_PIN               8

#else

#define MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define MAX_DIG_INPUT_CHANNEL   2       // Mechanical switches
#define MAX_PPM_OUTPUT_CHANNEL  6       // 17 channels max with Jeti module

#define PPM_PIN                 10

#define SWITCH1_PIN             2
#define SWITCH2_PIN             3

#define LED_PIN                 13

#define BT_RX_PIN               9
#define BT_TX_PIN               8

#endif


//
//  HF Modules
//

#ifdef JETI_TU2_MODULE

#define PPM_SHAPE_SIGNAL        1       // 0 is positive, 1 is negative
#define PPM_INTER_CHANNEL_TIME  500     // microsec
#define PPM_INTER_FRAME_TIME    4500    // microsec

#endif

//
// Never modify lower defines
//

#define MAX_INPUT_CHANNEL       MAX_ADC_INPUT_CHANNEL+MAX_DIG_INPUT_CHANNEL
#define STDOUT *SerialLink::currentStream_

#endif

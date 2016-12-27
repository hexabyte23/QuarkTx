/*
config.h - QuarkTx
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

#define QUARKTX_VERSION                   "0.3.1"
#define QUARKTX_PRIM_SERIAL               Serial
#define QUARKTX_PRIM_SERIAL_SPEED         9600

#define QUARKTX_MAX_MODEL                 2
#define QUARKTX_MAX_SERIAL_BUFFER         40      // warning: dont set too small value for RCL commands

#define QUARKTX_ADC_MIN_VALUE             0
#define QUARKTX_ADC_MAX_VALUE             1023

#define QUARKTX_PPM_MIN_VALUE             500
#define QUARKTX_PPM_MAX_VALUE             1500

#define QUARKTX_TX_MODE                   1       // mode 1 : left stick operates elevators & rudder,   right stick operates throttle & ailerons
                                                  // mode 2 : left stick operates throttle & rudder,    right stick operates elevators & ailerons
                                                  // mode 3 : left stick operates elevators & ailerons, right stick operates throttle & rudder
                                                  // mode 4 : left stick operates throttle & ailerons,  right stick operates elevators & rudder

#define QUARKTX_LED_BLINK_PERIOD          500     // millisec

#define BATTERY_RATE_PERIOD               1000    // check battery level every 1000 mSec
#define BATTERY_HISTO_BUFFER_SIZE         16      // for average value computing (warning the higher value the more RAM is consumed, Nano memory is tiny)

  /*
   * R1, R2 resistor choices
   * 
   * For Arduino Nano
   * ----------------
   * 
   * Analog pins: VCC = 5v
   * 2S battery: VCC = 7.40v, R1 = 47000 Ohm, R2 = 95300 Ohm 1%
   * 3S battery: VCC = 11.1v, R1 = 47000 Ohm, R2 = 38300 Ohm 1%
   * 
   * For Teensy 3.2
   * --------------
   * Analog pins: VCC = 3.3v
   * 2S battery: VCC = 7.40v, R1 = 47000 Ohm, R2 = 37400 Ohm 1%
   * 3S battery, VCC = 11.1v, R1 = 27000 Ohm, R2 = 11000 Ohm 1%
   * 
   */
 
#define QUARKTX_BATTERY_R1                27000  // Must be modity to you R1 resistor, ex: 27k Ohm
#define QUARKTX_BATTERY_R2                11000  // Must be modity to you R2 resistor, ex: 11k Ohm
#define QUARKTX_BATTERY_ALARM_LEVEL       9.60   // Raise alarm when Vcc batt is lower than 9.60 v


//
//
//  Put here after all definitions relative to your configuration
//
//

//#define QUARKTX_TERRATOP
#define QUARKTX_NEWRADIO
          
#define QUARKTX_JETI_TU2_MODULE



#ifdef QUARKTX_TERRATOP

#define QUARKTX_MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define QUARKTX_MAX_DIG_INPUT_CHANNEL   3       // Mechanical switches

#define QUARKTX_PPM_PIN                 10

#define QUARKTX_SWITCH1_PIN             2
#define QUARKTX_SWITCH2_PIN             3
#define QUARKTX_SWITCH3_PIN             4

#define QUARKTX_LED_PIN                 6

#elif defined(QUARKTX_NEWRADIO)

#define QUARKTX_MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define QUARKTX_MAX_DIG_INPUT_CHANNEL   3       // Mechanical switches

#define QUARKTX_PPM_PIN                 9

#define QUARKTX_SWITCH1_PIN             2
#define QUARKTX_SWITCH2_PIN             3
#define QUARKTX_SWITCH3_PIN             4

#define QUARKTX_LED_PIN                 0

#define QUARKTX_SPI_CS1_PIN             10

#else // generic radio

#define QUARKTX_MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define QUARKTX_MAX_DIG_INPUT_CHANNEL   2       // Mechanical switches

#define QUARKTX_PPM_PIN                 10

#define QUARKTX_SWITCH1_PIN             2
#define QUARKTX_SWITCH2_PIN             3

#define QUARKTX_LED_PIN                 13

#endif

//
//  HF Modules
//

#ifdef QUARKTX_JETI_TU2_MODULE

#define QUARKTX_MAX_PPM_OUTPUT_CHANNEL  6       // 17 channels max with Jeti TU2 module
#define QUARKTX_PPM_SHAPE_SIGNAL        HIGH    // LOW is positive, HIGH is negative
#define QUARKTX_PPM_INTER_CHANNEL_TIME  500     // microsec
#define QUARKTX_PPM_INTER_FRAME_TIME    4500    // microsec

#else // generic module

#define QUARKTX_MAX_PPM_OUTPUT_CHANNEL  4       
#define QUARKTX_PPM_SHAPE_SIGNAL        HIGH    // LOW is positive, HIGH is negative
#define QUARKTX_PPM_INTER_CHANNEL_TIME  500     // microsec
#define QUARKTX_PPM_INTER_FRAME_TIME    12000   // microsec

#endif

//////////////////////////////////////////
//    WARNING Never modify lower defines
//////////////////////////////////////////

#define QUARKTX_EEPROM_FORMAT           0x0001   // Format version for prevents format collision (lost of data)

#define QUARKTX_MAX_INPUT_CHANNEL       QUARKTX_MAX_ADC_INPUT_CHANNEL + QUARKTX_MAX_DIG_INPUT_CHANNEL
#define STDOUT                          *SerialLink::currentStream_

#ifdef __MK20DX256__
#define QUARKTX_TEENSY
#define QUARKTX_VREF 3.3
#define QUARKTX_ALT_SERIAL              Serial3
#define QUARKTX_ALT_SERIAL_SPEED        9600
#else
#define QUARKTX_NANO
#define QUARKTX_VREF 5.0
#endif

#ifdef QT_CORE_LIB
#define QUARKTX_TEST
#endif

#endif

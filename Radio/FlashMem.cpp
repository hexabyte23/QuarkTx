/*
FlashMem.cpp - QuarkTx
Copyright (c) 2015 Thierry & Betrand WILMOT.  All rights reserved.

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

#include <avr/pgmspace.h>
#include <stdarg.h>
#include <stdio.h>
#include "FlashMem.h"

// Define all static messages in flash memory instead of RAM

// Define all error messages
const char err_string_0[] PROGMEM = "[e] bad command format\n";
const char err_string_1[] PROGMEM = "[e] bad parameter: index (%d) >= max %d\n";
const char err_string_2[] PROGMEM = "[e] bad parameter: index empty\n";
const char err_string_3[] PROGMEM = "[e] buffer oversize\n";
const char err_string_4[] PROGMEM = "[e] EEPROM data is corrupted\n";
const char err_string_5[] PROGMEM = "[e] Command '%s' unknown\n";
const char err_string_6[] PROGMEM = "[e] Command string '%s' too long\n";

// global error tab
const char* const errorMsgTab[] PROGMEM = {err_string_0, err_string_1, err_string_2, err_string_3, err_string_4, err_string_5, err_string_6};

// Define all information messages
const char info_string_0[] PROGMEM = "Mode switched to 'transmit'\n";
const char info_string_1[] PROGMEM = "Mode switched to 'debug'\n";
const char info_string_2[] PROGMEM = "Current mode 'debug'\n";
const char info_string_3[] PROGMEM = "Current mode 'transmit'\n";
const char info_string_4[] PROGMEM = "help command :\n"
         "h: help\n"
         "m: toggle Tx mode\n"
         "l [0..2]: change cur. model\n"
         "d [0..2]: dump model\n"
         "i: toggle displ. input update\n"
         "o: toggle displ. output update\n"
         "a: load cur. model from EEPROM\n"
         "v: save cur. model to EEPROM\n";
const char info_string_5[] PROGMEM =
         "c: toggle calibrate sensors\n"     
         "s i [chan] [val]: set cur. model servo min\n"
         "s a [chan] [val]: set cur. model servo max\n"
         "s n [chan] [val]: set cur. model servo neutral\n"
         "s r [chan] [val]: set cur. model servo revert\n"
         "r: reset, clean all models\n";
const char info_string_19[] PROGMEM =
         "u: [sensorID] [val] set simulate value\n"
         "w: toggle to simulation\n";
const char info_string_6[] PROGMEM = "Current model saved to EEPROM\n";
const char info_string_7[] PROGMEM = "Current model load from EEPROM\n";
const char info_string_8[] PROGMEM = "Set Min channel %d value %d\n";
const char info_string_9[] PROGMEM = "Set Max channel %d value %d\n";
const char info_string_10[] PROGMEM = "Set Neutral channel %d value %d\n";
const char info_string_11[] PROGMEM = "Set Revert channel %d value %d\n";
const char info_string_12[] PROGMEM = "Servo\n# Min   Max   Neutral   Rev\n";
const char info_string_13[] PROGMEM = "Sensors (%d)\n# Pin   Trim    Min     Max    Simu\n";
const char info_string_14[] PROGMEM = "Quark Tx v%s\nBooting...\n";
const char info_string_15[] PROGMEM = "Command\tOK\n";
const char info_string_16[] PROGMEM = "Serial\tOK\n";
const char info_string_17[] PROGMEM = "Load model %d\n";
const char info_string_18[] PROGMEM = "Tx\tOK\t%d µs\n";
const char info_string_20[] PROGMEM = "Bluetooth\tOK\t\n";

// global error tab
const char* const infoMsgTab[] PROGMEM = {info_string_0, info_string_1, info_string_2, info_string_3, info_string_4, info_string_5, 
                                          info_string_6, info_string_7, info_string_8, info_string_9, info_string_10, info_string_11, 
                                          info_string_12, info_string_13, info_string_14, info_string_15, info_string_16, info_string_17,
                                          info_string_18, info_string_19, info_string_20};


// global functions
#define MAX_FM_BUFFER 240
static char fm_buffer[MAX_FM_BUFFER];

void error(int idx, ...)
{
  va_list args;

  strcpy_P(fm_buffer, (char*)pgm_read_word(&errorMsgTab[idx]));
  va_start (args, idx);
  vprintf(fm_buffer, args);
  va_end (args); 
}

void info(int idx, ...)
{
  va_list args;

  const char* ptr = (char*)pgm_read_word(&infoMsgTab[idx]);
  //Serial.print(strlen_P(ptr), DEC);
  if(strlen_P(ptr) < MAX_FM_BUFFER)
  {
    strcpy_P(fm_buffer, ptr);
    va_start (args, idx);
    vprintf(fm_buffer, args);
    va_end (args);
  }
  else
    error(ERR_BUFFER_OVERSIZE);
}

void debug(const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);
  vprintf(fmt, args);
  va_end (args);
}


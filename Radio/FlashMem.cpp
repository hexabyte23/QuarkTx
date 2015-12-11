#include <arduino.h> 
#include "FlashMem.h"

// Define all static messages in flash memory instead of RAM

// Define all error messages
const char err_string_0[] PROGMEM = "[e] bad command format\n";
const char err_string_1[] PROGMEM = "[e] bad parameter: index (%d) >= max %d\n";
const char err_string_2[] PROGMEM = "[e] bad parameter: index empty\n";
const char err_string_3[] PROGMEM = "[e] buffer oversize\n";
const char err_string_4[] PROGMEM = "[e] EEPROM data is corrupted\n";
const char err_string_5[] PROGMEM = "[e] Command '%s' unknown\n";

// global error tab
const char* const errorMsgTab[] PROGMEM = {err_string_0, err_string_1, err_string_2, err_string_3, err_string_4, err_string_5};

// Define all information messages
const char info_string_0[] PROGMEM = "Mode switched to 'transmit'\n";
const char info_string_1[] PROGMEM = "Mode switched to 'settings'\n";
const char info_string_2[] PROGMEM = "Current mode 'settings'\n";
const char info_string_3[] PROGMEM = "Current mode 'transmit'\n";
const char info_string_4[] PROGMEM = "help command :\n"
         "h: help\n"
         "m: toggle Tx mode\n"
         "l [0..4]: change cur. model\n"
         "d [0..4]: dump model\n"
         "i: toggle input update\n"
         "o: toggle output update\n"
         "a: load cur. modelfrom EEPROM\n"
         "v: save cur. model to EMPROM\n"
         "s i [chan] [val]: set cur. model servo min\n"
         "s a [chan] [val]: set cur. model servo max\n"
         "s t [chan] [val]: set cur. model servo trim\n"
         "s r [chan] [val]: set cur. model servo revert\n"
         "r: reset, clean all models\n"
         ;
const char info_string_5[] PROGMEM = "Current model saved to EEPROM\n";
const char info_string_6[] PROGMEM = "Current model load from EEPROM\n";
const char info_string_7[] PROGMEM = "Set Min channel %d value %d\n";
const char info_string_8[] PROGMEM = "Set Max channel %d value %d\n";
const char info_string_9[] PROGMEM = "Set Trim channel %d value %d\n";
const char info_string_10[] PROGMEM = "Set Revert channel %d value %d\n";

// global error tab
const char* const infoMsgTab[] PROGMEM = {info_string_0, info_string_1, info_string_2, info_string_3, info_string_4, info_string_5, 
                                          info_string_6, info_string_7, info_string_8, info_string_9, info_string_10};


// global functions
#define MAX_FM_BUFFER 400
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


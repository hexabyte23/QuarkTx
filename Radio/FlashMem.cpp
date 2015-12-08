#include <arduino.h> 
#include "FlashMem.h"

// Define all static messages in flash memory instead of RAM

// Define all error messages
const char err_string_0[] PROGMEM = "[e] bad command format\n";
const char err_string_1[] PROGMEM = "[e] bad parameter: expected 't' or 's'\n";
const char err_string_2[] PROGMEM = "[e] bad parameter: index (%d) >= max %d\n";
const char err_string_3[] PROGMEM = "[e] bad parameter: index empty\n";
const char err_string_4[] PROGMEM = "[e] buffer oversize\n";

// global error tab
const char* const errorMsgTab[] PROGMEM = {err_string_0, err_string_1, err_string_2, err_string_3, err_string_4};

// Define all information messages
const char info_string_0[] PROGMEM = "Mode switched to 'transmit'\n";
const char info_string_1[] PROGMEM = "Mode switched to 'settings'\n";
const char info_string_2[] PROGMEM = "Current mode 'settings'\n";
const char info_string_3[] PROGMEM = "Current mode 'transmit'\n";
const char info_string_4[] PROGMEM = "help command :\n"
         "h: help\n"
         "m: show current mode\n"
         "m 's'|'t': set mode to settings or transmit\n"
         "l [0..8]: change current model\n"
         "p [0..8]: dump model\n"
         "a: toggle start/stop update\n"
         "c: calibrate analogic sensors\n";

// global error tab
const char* const infoMsgTab[] PROGMEM = {info_string_0, info_string_1, info_string_2, info_string_3, info_string_4};


// global functions
#define MAX_FM_BUFFER 250
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


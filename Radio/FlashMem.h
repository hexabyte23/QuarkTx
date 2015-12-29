/*
FlashMem.h - QuarkTx
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

#ifndef FLASHMEM_H
#define FLASHMEM_H

#define ERR_BAD_COMMAND               0
#define ERR_BAD_PARAM_IDX_HIGH        1
#define ERR_BAD_PARAM_IDX_EMPTY       2
#define ERR_BUFFER_OVERSIZE           3
#define ERR_EEPROM_DATA_CORRUPTED     4
#define ERR_COMMAND_UNKNOWN           5
#define ERR_STRING_TOO_LONG           6
#define ERR_DEBUG_FIRST               7
#define ERR_LEFT_OP_EMPTY             8

#define INFO_SWITCH_MODE_TRANSMIT     0
#define INFO_SWITCH_MODE_SETTINGS     1
#define INFO_XXX1                     2
#define INFO_XXX2                     3
#define INFO_HELP_USAGE1              4
#define INFO_HELP_USAGE2              5
#define INFO_SAVE_TO_EEPROM           6
#define INFO_LOAD_FROM_EEPROM         7
#define INFO_SET_MIN_CHANNEL          8
#define INFO_SET_MAX_CHANNEL          9
#define INFO_SET_NEUTRAL_CHANNEL      10
#define INFO_SET_REVERT_CHANNEL       11
#define INFO_DUMP_SERVO_HEADER        12
#define INFO_SENSOR                   13
#define INFO_BOOTING_MESSAGE          14
#define INFO_COMMAND                  15
#define INFO_SERIAL                   16
#define INFO_LOAD_MODEL               17
#define INFO_TX_READY                 18
#define INFO_HELP_USAGE3              19
#define INFO_BT_READY                 20

void error(int idx, ...);
void info(int idx, ...);
void debug(const char *fmt, ...);

#endif

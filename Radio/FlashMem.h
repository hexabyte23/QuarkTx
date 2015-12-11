#ifndef FLASHMEM_H
#define FLASHMEM_H

#define ERR_BAD_COMMAND               0
#define ERR_BAD_PARAM_IDX_HIGH        1
#define ERR_BAD_PARAM_IDX_EMPTY       2
#define ERR_BUFFER_OVERSIZE           3
#define ERR_EEPROM_DATA_CORRUPTED     4
#define ERR_COMMAND_UNKNOWN           5

#define INFO_SWITCH_MODE_TRANSMIT     0
#define INFO_SWITCH_MODE_SETTINGS     1
#define INFO_CURRENT_MODE_TRANSMIT    2
#define INFO_CURRENT_MODE_SETTINGS    3
#define INFO_HELP_USAGE               4
#define INFO_SAVE_TO_EEPROM           5
#define INFO_LOAD_FROM_EEPROM         6
#define INFO_SET_MIN_CHANNEL          7
#define INFO_SET_MAX_CHANNEL          8
#define INFO_SET_TRIM_CHANNEL         9
#define INFO_SET_REVERT_CHANNEL       10
#define INFO_DUMP_SERVO_HEADER        11

void error(int idx, ...);
void info(int idx, ...);
void debug(const char *fmt, ...);

#endif

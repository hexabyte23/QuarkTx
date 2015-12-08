#ifndef FLASHMEM_H
#define FLASHMEM_H

#define ERR_BAD_COMMAND         0
#define ERR_BAD_PARAM_EXP_TS    1
#define ERR_BAD_PARAM_IDX_HIGH  2
#define ERR_BAD_PARAM_IDX_EMPTY 3
#define ERR_BUFFER_OVERSIZE     4

#define INFO_SWITCH_MODE_TRANSMIT     0
#define INFO_SWITCH_MODE_SETTINGS     1
#define INFO_CURRENT_MODE_TRANSMIT    2
#define INFO_CURRENT_MODE_SETTINGS    3
#define INFO_HELP_USAGE               4

void error(int idx, ...);
void info(int idx, ...);
void debug(const char *fmt, ...);

#endif

#ifndef _CRETRO_SRC_CLI_H_
#define _CRETRO_SRC_CLI_H_

#include <stdint.h>

typedef struct config {
    int debug;
    int us_delay;
    const char* rom;
} config_t;

config_t cli_config_default(void);

int cli_config_handle(config_t* const conf, int argc, char **argv);

#endif /* _CRETRO_SRC_CLI_H_ */

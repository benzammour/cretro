#ifndef _CRETRO_SRC_CLI_H_
#define _CRETRO_SRC_CLI_H_

#include <stdint.h>

typedef struct config {
    int debug;
    int us_delay;
    const char* rom_path;
} config_t;

config_t* cli_config_default(void);

void cli_config_destroy(config_t *conf);

int cli_config_handle(config_t* const conf, int argc, char **argv);

#endif /* _CRETRO_SRC_CLI_H_ */

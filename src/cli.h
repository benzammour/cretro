/**
 *
 * @file	cli.h
 * @brief 	Header defining stuff for command line argument parsing 
 * @author 	Moritz R. Huellmann
 * @date 	10.10.22
 *
 */

#ifndef _CRETRO_SRC_CLI_H_
#define _CRETRO_SRC_CLI_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct config {
    bool debug;
    uint16_t delay;
    const char* rom;
} config_t;

config_t cli_config_default(void);

int cli_config_handle(config_t* const conf, int argc, char **argv);

#endif /* _CRETRO_SRC_CLI_H_ */

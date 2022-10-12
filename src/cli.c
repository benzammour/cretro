#include "cli.h"

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

static const char* usage_str = "";

__attribute__((__const__)) config_t cli_config_default(void) {
    config_t conf = {
        .debug = false,
        .delay = 0,
        .rom = "",
    };

    return conf;
}

int cli_config_handle(config_t* const conf, int argc, char **argv) {
    int c;

    // parse all options first
    while ((c = getopt(argc, argv, "d")) != -1) {
        switch (c) {
            case 'd':
                conf->debug = true;
                break;
            default:
                fprintf(stderr, "%s\n", usage_str);
                return EXIT_FAILURE;
        }
    }

    // parse the remaining options
    long delay = strtol(argv[optind++], NULL, 10);
    if (delay > UINT16_MAX) {
        fprintf(stderr, "Provided delay %ld is higher than allowed delay %d\n", delay, UINT16_MAX);
        return EXIT_FAILURE;
    }

    // delay
    conf->delay = (uint16_t)delay;

    // path to rom
    conf->rom = argv[optind++];

    return EXIT_SUCCESS;
}

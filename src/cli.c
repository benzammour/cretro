#include "cli.h"

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

#define MAX_HZ (1000000L)

static const char* usage_str = "";

__attribute__((__const__)) config_t cli_config_default(void) {
    config_t conf = {
        .debug = false,
        .us_delay = 0,
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

    // calculate delay from hertz input
    long hertz = strtol(argv[optind++], NULL, 10);
    if (hertz > MAX_HZ) {
        fprintf(stderr, "Provided delay %ld is higher than allowed delay %ld\n", hertz, MAX_HZ);
        return EXIT_FAILURE;
    }

    // delay
    conf->us_delay = (uint32_t) (hertz / MAX_HZ);

    // path to rom
    conf->rom = argv[optind++];

    return EXIT_SUCCESS;
}

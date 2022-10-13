#include "cli.h"
#include "logging.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <getopt.h>
#include <stdio.h>

#define MAX_HZ (1000000L)

static const char* usage_str = "cretro [-d <0..5>] <HERTZ> <ROM>";

__attribute__((__const__)) config_t cli_config_default(void) {
    config_t conf = {
        .debug = 0,
        .us_delay = -1,
        .rom = "",
    };

    return conf;
}

int cli_config_handle(config_t* const conf, int argc, char **argv) {
    int c;

    // parse all options first
    while ((c = getopt(argc, argv, "d:")) != -1) {
        switch (c) {
            case 'd':
                conf->debug = (int) strtol(optarg, NULL, 10);
                log_init(conf);
                break;
            default:
                fprintf(stderr, "%s\n", usage_str);
                return EXIT_FAILURE;
        }
    }

    // parse the remaining options: <HERTZ> <ROM>

    // calculate delay from hertz input
    long hertz = strtol(argv[optind++], NULL, 10);
    LOG_DEBUG("Frequency [Hz] input specified: %ld.", hertz);

    if (hertz > MAX_HZ) {
        LOG_FATAL("Provided frequency %ld is higher than allowed frequency %ld", hertz, MAX_HZ);
        return EXIT_FAILURE;
    }
    else if (hertz <= 0) {
        LOG_FATAL("Provided frequency %ld is lower or equal to 0", hertz);
        return EXIT_FAILURE;
    }

    // delay
    if (optind >= argc) {
        LOG_FATAL("No frequency [Hz] has been specified!");
        return EXIT_FAILURE;
    }

    long delay = MAX_HZ / hertz;
    if (delay > INT_MAX) {
        LOG_FATAL("Delay is out of bounds: %d > %d", delay, INT_MAX);
        return EXIT_FAILURE;
    }

    conf->us_delay = (int) (delay);
    LOG_INFO("Delay set to %d us.", conf->us_delay);

    // path to rom
    if (optind >= argc) {
        LOG_FATAL("No ROM path specified!");
        return EXIT_FAILURE;
    }
    conf->rom = argv[optind++];
    LOG_DEBUG("Path to ROM: %s", conf->rom);

    return EXIT_SUCCESS;
}

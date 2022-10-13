#include "cli.h"
#include "logging.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <getopt.h>
#include <stdio.h>

#define MAX_HZ (1000000L)

static const char* usage_str = "cretro [-d <0..5>] [-f <FREQUENCY_HZ>] <ROM>";

static void _handle_arg_frequency(config_t* conf, long frequency) {
    // calculate delay from hertz input
    LOG_DEBUG("Frequency [Hz] input specified: %ld.", frequency);

    if (frequency > MAX_HZ) {
        LOG_ERROR("Provided frequency %ld is higher than allowed frequency %ld. Used Fallback: %ld", frequency, MAX_HZ, MAX_HZ);
        frequency = MAX_HZ;
    }
    else if (frequency <= 0) {
        LOG_ERROR("Provided frequency %ld is lower or equal to 0. Used Fallback: %ld", frequency, MAX_HZ);
        frequency = MAX_HZ;
    }

    long delay = MAX_HZ / frequency;
    if (delay > INT_MAX)
        LOG_FATAL("Delay is out of bounds: %d > %d", delay, INT_MAX);

    conf->us_delay = (int) (delay);
    LOG_INFO("Delay set to %d us.", conf->us_delay);
}

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

    if (argc < 2) {
        fprintf(stderr, "%s\n", usage_str);
        return EXIT_FAILURE;
    }

    // parse all options first
    while ((c = getopt(argc, argv, "d:f:")) != -1) {
        switch (c) {
            case 'd':
                conf->debug = (int) strtol(optarg, NULL, 10);
                log_init(conf);
                break;
            case 'f':
                _handle_arg_frequency(conf, strtol(optarg, NULL, 10));
                break;
            default:
                fprintf(stderr, "%s\n", usage_str);
                return EXIT_FAILURE;
        }
    }

    // parse the remaining options: <HERTZ> <ROM>

    // calculate delay from hertz input

    // delay
    if (optind >= argc) {
        LOG_FATAL("No frequency [Hz] has been specified!");
        return EXIT_FAILURE;
    }

    // path to rom
    if (optind >= argc) {
        LOG_FATAL("No ROM path specified!");
        return EXIT_FAILURE;
    }
    conf->rom = argv[optind++];
    LOG_DEBUG("Path to ROM: %s", conf->rom);

    return EXIT_SUCCESS;
}

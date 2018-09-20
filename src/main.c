#include "main.h"

signed
main (signed argc, const char * argv []) {

    signed status = EXIT_SUCCESS;

    READ_ENV(dev, DEVICE);
    READ_ENV(devpath, SEARCH_PATH);
    READ_ENV(thresh_top, THRESHOLD_MAX);
    READ_ENV(thresh_bot, THRESHOLD_MIN);
    READ_ENV(tran_steps, TRANSITION_STEPS);
    READ_ENV(tran_pause, TRANSITION_PAUSE);

    size_t pathlen = strlen(devpath) + 1;

    unsigned parsed_steps = 0;
    sscanf(tran_steps, "%u", &parsed_steps);
    parsed_steps += !parsed_steps;

    unsigned parsed_pause = 0;
    sscanf(tran_pause, "%u", &parsed_pause);

    size_t blen = pathlen + strlen(dev) + sizeof "/brightness";
    char * bpath = malloc(blen);
    if ( !bpath ) {
        fputs(FAILED_TO "allocate space for brightness path\n", stderr);
    }

    size_t mlen = pathlen + strlen(dev) + sizeof "/max_brightness";
    char * mpath = malloc(mlen);
    if ( !mpath ) {
        fputs(FAILED_TO "allocate space for max brightness path\n", stderr);
    }

    if ( !bpath || !mpath ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    snprintf(bpath, blen, "%s/%s/brightness", devpath, dev);
    snprintf(mpath, mlen, "%s/%s/max_brightness", devpath, dev);

    unsigned cur = 0, max = 0;
    if ( argc < 2 ) {
        cur = bl_get(bpath);
        max = bl_get(mpath);
        printf("%u / %u (%u%%)\n", cur, max, cur * 100 / max);
        goto cleanup;
    } else {
        if ( argv[1][0] == 'h' ) {
            fputs(USAGE_STR, stderr);
            goto cleanup;
        } else if ( argv[1][0] == 'l' ) {
            bl_list(devpath);
            goto cleanup;
        }
    }

    if ( !cur || !max ) {
        cur = bl_get(bpath);
        max = bl_get(mpath);
    }

    struct brightness_cmd cmd = bl_cmd_parse(argv[1]);
    if ( cmd.sign == '!' ) {
        fputs(USAGE_STR, stderr);
        status = EXIT_FAILURE;
        goto cleanup;
    }

    unsigned ciel = bl_calc(bl_cmd_parse(thresh_top), 0, 0, max),
             floo = bl_calc(bl_cmd_parse(thresh_bot), 0, 0, ciel);

    unsigned nbness = bl_calc(cmd, cur, floo, ciel);
    bool direction = nbness > cur;
    unsigned step = (direction ? nbness - cur : cur - nbness) / parsed_steps;
    step += !step;

    const struct timespec pause_time = {
        .tv_sec = 0, .tv_nsec = parsed_pause
    };

    for ( size_t i = 0; i < parsed_steps; ++ i ) {
        cur = (direction ? cur + step : cur - step);
        bl_set(bpath, cur);
        if ( (direction ? cur >= nbness : cur <= nbness) ) { break; }
        nanosleep(&pause_time, NULL);
    }

    cleanup:
        if ( bpath ) { free(bpath); }
        if ( mpath ) { free(mpath); }
        return status;
 }


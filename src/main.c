#include "main.h"

signed
main (signed argc, const char * argv []) {

    signed status = EXIT_SUCCESS;

    const char * dev = 0;
    dev = getenv("BACKLIGHT_DEVICE");
    dev = dev ? dev : D_DEV;

    const char * devpath = 0;
    devpath = getenv("BACKLIGHT_SEARCH_PATH");
    devpath = devpath ? devpath : BASEPATH;
    size_t pathlen = strlen(devpath) + 1;

    const char * thresh_top = 0;
    thresh_top = getenv("BACKLIGHT_THRESHOLD_MAX");
    thresh_top = thresh_top ? thresh_top : THRESH_TOP;

    const char * thresh_bot = 0;
    thresh_bot = getenv("BACKLIGHT_THRESHOLD_MIN");
    thresh_bot = thresh_bot ? thresh_bot : THRESH_BOT;

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
    unsigned step = (direction ? nbness - cur : cur - nbness) / TRAN_STEPS;
    step += !step;

    for ( size_t i = 0; i < TRAN_STEPS; ++ i ) {
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

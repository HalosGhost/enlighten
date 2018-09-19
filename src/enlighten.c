#include "enlighten.h"

void
bl_set (const char * path, unsigned level) {

    FILE * fp = fopen(path, "w");
    if ( !fp ) {
        fprintf(stderr, FAILED_TO "open %s for reading: %s\n", path,
                strerror(errno));
        exit(EXIT_FAILURE);
    } fprintf(fp, "%u", level); fclose(fp);
}

unsigned
bl_get (const char * path) {

    FILE * fp = fopen(path, "r");
    if ( !fp ) {
        fprintf(stderr, FAILED_TO "open %s for reading: %s\n", path,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    unsigned bness = 0;
    if ( fscanf(fp, "%u", &bness) != 1 ) {
        fprintf(stderr, FAILED_TO "read from %s: %s\n", path, strerror(errno));
        fclose(fp);
        exit(EXIT_FAILURE);
    } fclose(fp);

    return bness;
}

struct brightness_cmd
bl_cmd_parse (const char * string) {

    char sign [] = { 0, 0 }, perc [] = { 0, 0 };
    signed bness = 0;

    sscanf(string, "%1[+-]", sign);
    if ( sscanf(string, "%d", &bness) != 1 ) {
        sign[0] = '!';
    } sscanf(string, "%*d%1[%]", perc);

    return (struct brightness_cmd ){ .bness = bness
                                   , .sign = sign[0]
                                   , .perc = perc[0] == '%'
                                   };
}

unsigned
bl_calc (struct brightness_cmd cmd, unsigned cur, unsigned min, unsigned max) {

    if ( cmd.perc ) {
        cmd.bness = cmd.bness * (signed )max / 100;
    }

    bool sign = cmd.sign == '-' || cmd.sign == '+';
    signed res = cmd.bness + sign * (signed )cur;

    return res > (signed )max ? max :
           res < (signed )min ? min : (unsigned )res;
}

void
bl_list (const char * devpath) {

    DIR * dir = opendir(devpath);

    if ( dir ) {
        for ( struct dirent * p = readdir(dir); p; p = readdir(dir) ) {
            if ( p->d_name[0] != '.' ) {
                printf("%s\t", p->d_name);
            }
        } putchar('\n');
    }

    closedir(dir);
}

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

    for ( size_t i = 0; i < TRAN_STEPS; ++ i ) {
        if ( direction ) {
            cur += step;
        } else {
            cur -= step;
        }

        bl_set(bpath, cur);
        sleep(TRAN_PAUSE);
    }

    cleanup:
        if ( bpath ) { free(bpath); }
        if ( mpath ) { free(mpath); }
        return status;
 }

// vim: set ts=4 sw=4 et:

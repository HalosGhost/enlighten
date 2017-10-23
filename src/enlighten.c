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

unsigned
bl_calc (signed bness, bool sign, bool perc, unsigned cur, unsigned max) {

    if ( perc ) {
        bness = bness * (signed )max / 100;
    }

    return (unsigned )(bness + sign * (signed )cur);
}

signed
main (signed argc, const char * argv []) {

    signed status = EXIT_SUCCESS;

    const char * dev = 0;
    dev = getenv("BACKLIGHT_DEVICE");
    dev = dev ? dev : D_DEV;

    size_t blen = 33 + strlen(dev),
           mlen = 37 + strlen(dev);

    char * bpath = malloc(blen),
         * mpath = malloc(mlen);
    if ( !bpath || !mpath ) {
        fputs(FAILED_TO "allocate space for device paths\n", stderr);
        status = EXIT_FAILURE;
        goto cleanup;
    }

    snprintf(bpath, blen, BASEPATH "%s/brightness", dev);
    snprintf(mpath, mlen, BASEPATH "%s/max_brightness", dev);

    unsigned cur = bl_get(bpath), max = bl_get(mpath);
    if ( argc < 2 ) {
        printf("%u / %u (%u%%)\n", cur, max, cur * 100 / max);
        goto cleanup;
    }

    char sign = 0, perc [] = { 0, 0 };
    signed bness = 0;
    sscanf(argv[1], "%[+-]", &sign);
    if ( sscanf(argv[1], "%d", &bness) != 1 ) {
        fputs(USAGE_STR, stderr);
        status = EXIT_FAILURE;
        goto cleanup;
    }

    sscanf(argv[1], "%*d%[%]", perc);

    unsigned nbness = bl_calc(bness, sign, perc[0] == '%', cur, max);
    bl_set(bpath, nbness);

    cleanup:
        if ( bpath ) { free(bpath); }
        if ( mpath ) { free(mpath); }
        return status;
 }

// vim: set ts=4 sw=4 et:

#include "enlighten.h"

void
bl_set (unsigned level) {

    FILE * fp = fopen(D_PATH, "w");
    if ( !fp ) {
        perror(FAILED_TO "open " D_PATH " for writing");
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

signed
main (signed argc, char * argv []) {

    if ( argc < 2 ) {
        printf("%u\n", bl_get(D_PATH));
        return EXIT_SUCCESS;
    }

    char sign = 0;
    unsigned bness = 0;
    sscanf(argv[1], "%[+-]", &sign);
    if ( sscanf(argv[1], "%u", &bness) != 1 ) {
        fputs(USAGE_STR, stderr);
        return EXIT_FAILURE;
    }

    if ( getuid() ) { fputs(PERMS_STR, stderr); exit(EXIT_FAILURE); }
    bl_set(bness + !!sign * bl_get(D_PATH));

    return EXIT_SUCCESS;
 }

// vim: set ts=4 sw=4 et:

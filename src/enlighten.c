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
bl_get (void) {

    FILE * fp = fopen(D_PATH, "r");
    if ( !fp ) {
        perror(FAILED_TO "open " D_PATH " for reading");
        exit(EXIT_FAILURE);
    }

    unsigned bness = 0;
    if ( fscanf(fp, "%u", &bness) != 1 ) {
        perror(FAILED_TO "read from " D_PATH);
        fclose(fp);
        exit(EXIT_FAILURE);
    } fclose(fp);

    return bness;
}

signed
main (signed argc, char * argv []) {

    if ( argc < 2 ) {
        printf("%u\n", bl_get());
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
    bl_set(sign ? bness + bl_get() : bness);

    return EXIT_SUCCESS;
 }

// vim: set ts=4 sw=4 et:

#include "enlighten.h"

void
bl_set (signed level) {

    level = level < 0 ? 0 : level;

    FILE * fp = fopen(D_PATH, "w");
    if ( !fp ) {
        perror(FAILED_TO "open " D_PATH " for writing");
        exit(EXIT_FAILURE);
    } fprintf(fp, "%d", level); fclose(fp);
}

signed
bl_get (void) {

    FILE * fp = fopen(D_PATH, "r");
    if ( !fp ) {
        perror(FAILED_TO "open " D_PATH " for reading");
        exit(EXIT_FAILURE);
    }

    signed bness = 0;
    if ( fscanf(fp, "%d", &bness) != 1 ) {
        perror(FAILED_TO "read from " D_PATH);
        fclose(fp);
        exit(EXIT_FAILURE);
    } fclose(fp);

    return bness;
}

void
bl_modify (char act) {

    signed bness = bl_get();
    bl_set(act == '+' ? bness + D_STEP : bness - D_STEP);
}

signed
main (signed argc, char * argv []) {

    if ( argc <= 1 ) {
        fputs(USAGE_STR, stderr);
        return EXIT_SUCCESS;
    } else if ( getuid() ) {
        fputs(PERMS_STR, stderr);
        return EXIT_FAILURE;
    }

    bl_modify(argv[1][0]);
    return EXIT_SUCCESS;
}

// vim: set ts=4 sw=4 et:

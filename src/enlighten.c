#include "enlighten.h"

void
modify (char act) {

    FILE * fp = fopen(D_PATH, "r");
    if ( !fp ) {
        perror(PROGNAME ": Failed to open " D_PATH);
        exit(EXIT_FAILURE);
    }

    signed bness = 0;
    if ( fscanf(fp, "%d", &bness) != 1 ) {
        perror(PROGNAME ": Failed to read from " D_PATH);
        fclose(fp);
        exit(EXIT_FAILURE);
    } fclose(fp);

    signed nb = act == '+' ? bness + D_STEP : bness - D_STEP;

    fp = fopen(D_PATH, "w");
    if ( !fp ) {
        perror(PROGNAME ": Failed to reopen " D_PATH);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%d", nb); fclose(fp);
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

    modify(argv[1][0]);
    return EXIT_SUCCESS;
}

// vim: set ts=4 sw=4 et:

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

signed
main (signed argc, char * argv []) {

    if ( argc <= 1 ) {
        fputs(USAGE_STR, stderr);
        return EXIT_SUCCESS;
    }

    switch ( argv[1][0] ) {
        case 'h': fputs(USAGE_STR, stdout);                 break;
        case 'd': check_perms(); bl_set(bl_get() - D_STEP); break;
        case 'i': check_perms(); bl_set(bl_get() + D_STEP); break;
        case 'g': printf("%d\n", bl_get());                 break;
        case 's': {
            check_perms(); signed bness = 0;
            if ( argc < 3 || sscanf(argv[2], "%d", &bness) != 1 ) {
                fputs(USAGE_STR, stderr);
                return EXIT_FAILURE;
            } else {
                bl_set(bness);
            } break;
        }
        default:  fputs(USAGE_STR, stderr); return EXIT_FAILURE;
    } return EXIT_SUCCESS;
 }

// vim: set ts=4 sw=4 et:

#include "enlighten.h"

void
bl_set (const char * path, unsigned level) {

    FILE * fp = fopen(path, "w");
    if ( !fp ) {
        fprintf(stderr, FAILED_TO "open %s for writing: %s\n", path,
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

    DIR * devdir = opendir(devpath);

    if ( devdir ) {
        for ( struct dirent * p = readdir(devdir); p; p = readdir(devdir) ) {
            size_t candidate_len = strlen(devpath) + strlen(p->d_name) + 2;
            char * candidate = malloc(candidate_len);
            snprintf(candidate, candidate_len, "%s/%s", devpath, p->d_name);
            DIR * candidate_dir = opendir(candidate);
            if ( candidate_dir ) {
                unsigned required_files = 0;
                for ( struct dirent * cp = readdir(candidate_dir); cp; cp = readdir(candidate_dir) ) {
                    required_files += !strcmp(cp->d_name, "brightness");
                    required_files += !strcmp(cp->d_name, "max_brightness");
                }
                if ( required_files == 2 ) {
                    printf("%s\t", p->d_name);
                }
                closedir(candidate_dir);
            }
            free(candidate);
        } putchar('\n');

        closedir(devdir);
    }
}


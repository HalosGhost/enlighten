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

    char sign [] = { 0, 0 };
    if ( sscanf(string, "%1[=+-]", sign) == 1 ) { ++ string; }

    signed bness = 0;
    if ( sscanf(string, "%d", &bness) != 1 ) { sign[0] = '!'; }

    if ( *sign == '-' ) { bness *= -1; }

    char perc [] = { 0, 0 };
    sscanf(string, "%*d%1[%]", perc);

    return (struct brightness_cmd ){
        .bness = bness,
        .sign = *sign,
        .perc = *perc == '%'
    };
}

unsigned
bl_calc (struct brightness_cmd cmd, unsigned cur, unsigned max, unsigned bot, unsigned top) {

    if ( cmd.perc ) {
        cmd.bness = cmd.bness * (signed )max / 100;
    }

    bool sign = cmd.sign == '-' || cmd.sign == '+';
    signed res = cmd.bness + sign * (signed )cur;

    return res > (signed )top ? top :
           res < (signed )bot ? bot : (unsigned )res;
}

void
bl_list (char *const * search_paths, size_t path_count) {

    for ( size_t i = 0; i < path_count; ++ i ) {
        const char * devpath = search_paths[i];

        DIR * devdir = opendir(devpath);

        if ( devdir ) {
            printf("%s:\n", devpath);
            bool has_candidates = false;
            struct dirent * p = 0;
            while ( (p = readdir(devdir)) ) {
                size_t candidate_len = strlen(devpath) + strlen(p->d_name) + 2;
                char * candidate = malloc(candidate_len);
                snprintf(candidate, candidate_len, "%s/%s", devpath, p->d_name);

                DIR * candidate_dir = opendir(candidate);
                if ( candidate_dir ) {
                    unsigned required_files = 0;

                    struct dirent * cp = 0;
                    while ( (cp = readdir(candidate_dir)) ) {
                        required_files += !strcmp(cp->d_name, "brightness") && cp->d_type == DT_REG;
                        required_files += !strcmp(cp->d_name, "max_brightness") && cp->d_type == DT_REG;
                    }

                    if ( required_files == 2 ) {
                        has_candidates = true;
                        printf("    %s\n", p->d_name);
                    }

                    closedir(candidate_dir);
                }

                free(candidate);
            }

            if ( !has_candidates ) {
                puts("    <No valid candidates>");
            }

            putchar('\n');
            closedir(devdir);
        }
    }
}


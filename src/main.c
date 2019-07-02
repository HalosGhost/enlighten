#include "main.h"

signed
main (signed argc, const char * argv []) {

    signed status = EXIT_SUCCESS;

    READ_ENV(dev, DEVICE);
    READ_ENV(devpath, SEARCH_PATH);
    READ_ENV(thresh_top, THRESHOLD_MAX);
    READ_ENV(thresh_bot, THRESHOLD_MIN);
    READ_ENV(tran_steps, TRANSITION_STEPS);
    READ_ENV(tran_pause, TRANSITION_PAUSE);

    size_t pathlen = strlen(devpath) + 1;

    char * pathcopy = 0, ** search_paths = 0, * mpath = 0, * bpath = 0;

    pathcopy = malloc(pathlen);
    if ( !pathcopy ) {
        fputs(FAILED_TO "allocate space for a copy of SEARCH_PATH\n", stderr);
        goto cleanup;
    }

    strncpy(pathcopy, devpath, pathlen);

    size_t path_count = 1;
    for ( char * c = pathcopy; *c; ++ c ) {
        path_count += *c == ':';
    }

    search_paths = malloc(sizeof (char *) * path_count);
    search_paths[path_count - 1] = 0;
    if ( !search_paths ) {
        fputs(FAILED_TO "allocate space for the search paths\n", stderr);
        goto cleanup;
    }

    search_paths[0] = strtok(pathcopy, ":");
    for ( size_t i = 1; i < path_count; ++ i ) {
        search_paths[i] = strtok(0, ":");
    }

    const char * target_path = 0;
    for ( size_t i = 0; i < path_count; ++ i ) {
        if ( !search_paths[i] ) {
            continue;
        }

        DIR * dir = opendir(search_paths[i]);

        if ( dir ) {
            struct dirent * p = 0;
            while ( (p = readdir(dir)) ) {
                target_path = !strncmp(p->d_name, dev, strlen(dev)) ? search_paths[i] : 0;
                if ( target_path ) { break; }
            }

            closedir(dir);
        }

        if ( target_path ) { break; }
    }

    if ( argc == 2 ) {
        switch ( argv[1][0] ) {
            case 'h':
                fputs(USAGE_STR, stderr);
                goto cleanup;
            case 'v':
                #if !defined(VERSION)
                #define VERSION "No version information available\n"
                #endif
                fputs(VERSION, stderr);
                goto cleanup;
            case 'l':
                bl_list(search_paths, path_count);
                goto cleanup;
        }
    }

    unsigned parsed_steps = 0;
    sscanf(tran_steps, "%u", &parsed_steps);
    parsed_steps += !parsed_steps;

    long parsed_pause = 0;
    sscanf(tran_pause, "%ld", &parsed_pause);
    parsed_pause *= !(parsed_pause < 0);

    if ( !target_path ) {
        fputs(FAILED_TO "find a match for the specified device\n", stderr);
        goto cleanup;
    }

    size_t blen = pathlen + strlen(dev) + sizeof "/brightness";
    bpath = malloc(blen);
    if ( !bpath ) {
        fputs(FAILED_TO "allocate space for brightness path\n", stderr);
    }

    size_t mlen = pathlen + strlen(dev) + sizeof "/max_brightness";
    mpath = malloc(mlen);
    if ( !mpath ) {
        fputs(FAILED_TO "allocate space for max brightness path\n", stderr);
    }

    if ( !bpath || !mpath ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    snprintf(bpath, blen, "%s/%s/brightness", target_path, dev);
    snprintf(mpath, mlen, "%s/%s/max_brightness", target_path, dev);

    unsigned cur = 0, max = 0;
    if ( argc < 2 ) {
        cur = bl_get(bpath);
        max = bl_get(mpath);
        printf("%u / %u (%u%%)\n", cur, max, cur * 100 / max);
        goto cleanup;
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

    unsigned ciel = bl_calc(bl_cmd_parse(thresh_top), 0, max, 0, max),
             floo = bl_calc(bl_cmd_parse(thresh_bot), 0, max, 0, ciel);

    unsigned nbness = bl_calc(cmd, cur, max, floo, ciel);
    bool direction = nbness > cur;
    unsigned step = (direction ? nbness - cur : cur - nbness) / parsed_steps;
    step += !step;

    const struct timespec pause_time = {
        .tv_sec = 0, .tv_nsec = parsed_pause
    };

    for ( size_t i = 0; i < parsed_steps; ++ i ) {
        cur = (direction ? cur + step : cur - step);
        bl_set(bpath, cur);
        if ( (direction ? cur >= nbness : cur <= nbness) ) { break; }
        nanosleep(&pause_time, NULL);
    }

    cleanup:
        if ( bpath ) { free(bpath); }
        if ( mpath ) { free(mpath); }
        if ( pathcopy ) { free(pathcopy); }
        if ( search_paths ) { free(search_paths); }
        return status;
 }


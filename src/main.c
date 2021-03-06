#include "main.h"

signed
main (signed argc, const char * argv []) {

    signed status = EXIT_SUCCESS;

    READ_ENV(defdev, DEVICE);
    READ_ENV(devpath, SEARCH_PATH);
    READ_ENV(thresh_top, THRESHOLD_MAX);
    READ_ENV(thresh_bot, THRESHOLD_MIN);
    READ_ENV(tran_steps, TRANSITION_STEPS);
    READ_ENV(tran_pause, TRANSITION_PAUSE);

    char dev [PATH_MAX] = "";
    strncpy(dev, defdev, PATH_MAX - 1);

    size_t pathlen = strlen(devpath) + 1;

    char * pathcopy = 0, ** search_paths = 0, * mpath = 0, * bpath = 0;

    pathcopy = malloc(pathlen);
    if ( !pathcopy ) {
        fputs(FAILED_TO "allocate space for a copy of SEARCH_PATH\n", stderr);
        goto cleanup;
    }

    strcpy(pathcopy, devpath);

    char * c = strtok(pathcopy, ":");
    if ( !c ) {
        fputs(PROGNAME ": SEARCH_PATH is effectively empty\n", stderr);
        goto cleanup;
    }

    size_t path_count = 1;
    while ( c && (c = strtok(0, ":")) ) {
        path_count++;
    }

    strncpy(pathcopy, devpath, pathlen);

    search_paths = malloc(sizeof (char *) * path_count);
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
                if ( target_path ) { break; }

                size_t candidate_len = strlen(devpath) + strlen(p->d_name) + 2;
                char * candidate = malloc(candidate_len);
                snprintf(candidate, candidate_len, "%s/%s", search_paths[i], p->d_name);

                if ( bl_path_is_device(candidate) && !strncmp(p->d_name, dev, strlen(dev)) ) {
                    target_path = search_paths[i];
                    strncpy(dev, p->d_name, PATH_MAX - 1);
                }

                free(candidate);
            }
        }

        if ( target_path ) { break; }
    }

    if ( argc == 2 ) {
        switch ( argv[1][0] ) {
            case 'e':
                PRINT_ENV(DEVICE);
                PRINT_ENV(SEARCH_PATH);
                PRINT_ENV(THRESHOLD_MAX);
                PRINT_ENV(THRESHOLD_MIN);
                PRINT_ENV(TRANSITION_STEPS);
                PRINT_ENV(TRANSITION_PAUSE);
                goto cleanup;
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
                bl_list(search_paths, path_count, dev);
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

    unsigned top = bl_calc(bl_cmd_parse(thresh_top), 0, max, 0, max),
             bot = bl_calc(bl_cmd_parse(thresh_bot), 0, max, 0, top);

    unsigned nbness = bl_calc(cmd, cur, max, bot, top);
    if ( nbness == cur ) {
        goto cleanup;
    }

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


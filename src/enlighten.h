#if !defined(ENLIGHTEN_H)
#define ENLIGHTEN_H

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define PROGNAME  "enlighten"
#define FAILED_TO PROGNAME ": Failed to "

struct brightness_cmd {
    signed bness;
    signed sign: sizeof (signed) * CHAR_BIT / 2,
           perc: sizeof (signed) * CHAR_BIT / 2;
};

void
bl_set (const char *, unsigned);

unsigned
bl_get (const char *);

struct brightness_cmd
bl_cmd_parse (const char *);

unsigned
bl_calc (struct brightness_cmd, unsigned, unsigned, unsigned, unsigned);

void
bl_list (char *const *, size_t);

#endif // ENLIGHTEN_H

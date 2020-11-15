#if !defined(MAIN_H)
#define MAIN_H

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#include "enlighten.h"
#include "config.h"

#define _POSIX_C_SOURCE 200809L

static const char USAGE_STR [] =
    "Usage: " PROGNAME " <command>\n"
    PROGNAME " -- a very small backlight controller\n\n"
    "Commands:\n"
    "  <nothing>        get current brightness\n"
    "  env              print environment configuration\n"
    "  help             print this help and exit\n"
    "  list             get a list of devices\n"
    "  version          print version information and exit\n"
    "  [=|+|-]<int>[%]  set brightness to int\n\n"
    "  if + or - is specified, increment or\n"
    "  decrement accordingly; = is allowed, but\n"
    "  ignored\n\n"
    "  if % is specified, treat int as a\n"
    "  percentage of the max brightness\n"
    "\nsee " PROGNAME "(1) for more information\n"
    "regarding usage and configuration\n"
    ;

#define READ_ENV(cvar, def) \
const char * cvar = 0; \
do { \
    (cvar) = getenv("BACKLIGHT_" #def); \
    (cvar) = (cvar) ? (cvar) : (def); \
} while ( false )

#define PRINT_ENV(def) \
    printf("BACKLIGHT_" #def ": %s (default: \"%s\")\n", getenv("BACKLIGHT_" #def), def)

#endif // MAIN_H

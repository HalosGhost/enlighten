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

#define PROGNAME  "enlighten"
#define FAILED_TO PROGNAME ": Failed to "

/* Configuration */
#define D_DEV    "intel_backlight"
#define BASEPATH "/sys/class/backlight"

#define THRESH_TOP "100%"
#define THRESH_BOT "0"
#define TRAN_STEPS 1
#define TRAN_PAUSE 0

#define _POSIX_C_SOURCE 200809L

static const struct timespec pause_time = {
    .tv_sec = 0, .tv_nsec = TRAN_PAUSE
};

static const char USAGE_STR [] =
    "Usage: " PROGNAME " <command>\n"
    PROGNAME " -- a very small backlight controller\n\n"
    "Commands:\n"
    "  <nothing>      get current brightness\n"
    "  list           get a list of devices\n"
    "  help           print this help and exit\n"
    "  [+|-]<int>[%]  set brightness to int\n\n"
    "  if + or - is specified, increment or\n"
    "  decrement accordingly\n\n"
    "  if % is specified, treat int as a\n"
    "  percentage of the max brightness\n"
    "\nsee " PROGNAME "(1) for more usage information\n"
    ;

#include "enlighten.h"

#endif // ENLIGHTEN_H
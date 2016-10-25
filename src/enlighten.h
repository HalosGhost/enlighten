#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PROGNAME  "enlighten"
#define PERMS_STR PROGNAME ": That command must be run with root permissions\n"
#define FAILED_TO PROGNAME ": Failed to "

/* Configuration */
#define D_STEP 100
#define D_DEV  "intel_backlight"
#define D_PATH "/sys/class/backlight/" D_DEV "/brightness"

static const char USAGE_STR [] =
    "Usage: " PROGNAME " <command> (<argument>)\n"
    PROGNAME " -- a very small backlight controller\n\n"
    "Commands:\n"
    "  help        print this help and exit\n"
    "  get         print the current brightness and exit\n"
    "  set LVL     set brightness to LVL and exit\n"
    "  inc         increase brightness by step\n"
    "  dec         decrease brightness by step\n";

void
bl_set (signed);

signed
bl_get (void);

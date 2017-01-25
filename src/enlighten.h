#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PROGNAME  "enlighten"
#define PERMS_STR PROGNAME ": That command must be run with root permissions\n"
#define FAILED_TO PROGNAME ": Failed to "

/* Configuration */
#define D_DEV  "intel_backlight"
#define D_PATH "/sys/class/backlight/" D_DEV "/brightness"

static const char USAGE_STR [] =
    "Usage: " PROGNAME " <command> (<argument>)\n"
    PROGNAME " -- a very small backlight controller\n\n"
    "Commands:\n"
    "  help           print this help and exit\n"
    "  [+|-]<int>[%]  set brightness to int\n\n"
    "  if + or - is specified, increment or\n"
    "  decrement accordingly\n\n"
    "  if % is specified, treat int as a\n"
    "  percentage of the max brightness\n";

void
bl_set (unsigned);

unsigned
bl_get (const char *);

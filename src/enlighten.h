#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define PROGNAME  "enlighten"
#define FAILED_TO PROGNAME ": Failed to "

/* Configuration */
#define D_DEV    "intel_backlight"
#define BASEPATH "/sys/class/backlight/"

static const char USAGE_STR [] =
    "Usage: " PROGNAME " <command>\n"
    PROGNAME " -- a very small backlight controller\n\n"
    "Commands:\n"
    "  <nothing>      get current brightness\n"
    "  help           print this help and exit\n"
    "  [+|-]<int>[%]  set brightness to int\n\n"
    "  if + or - is specified, increment or\n"
    "  decrement accordingly\n\n"
    "  if % is specified, treat int as a\n"
    "  percentage of the max brightness\n\n"
    "set the BACKLIGHT_DEVICE environment variable\n"
    "to specify a device name at runtime\n";

void
bl_set (const char *, unsigned);

unsigned
bl_get (const char *);

unsigned
bl_calc (signed, bool, bool, unsigned, unsigned);


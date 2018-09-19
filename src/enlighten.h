#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>

#define PROGNAME  "enlighten"
#define FAILED_TO PROGNAME ": Failed to "

/* Configuration */
#define D_DEV    "intel_backlight"
#define BASEPATH "/sys/class/backlight"

#define THRESH_TOP "100%"
#define THRESH_BOT "0"
#define TRAN_STEPS 1
#define TRAN_PAUSE 0

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

struct brightness_cmd {
    signed bness;
    signed sign: 16, perc: 16;
};

void
bl_set (const char *, unsigned);

unsigned
bl_get (const char *);

struct brightness_cmd
bl_cmd_parse (const char *);

unsigned
bl_calc (struct brightness_cmd, unsigned, unsigned, unsigned);

void
bl_list (const char *);

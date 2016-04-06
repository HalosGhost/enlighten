#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PROGNAME  "enlighten"
#define USAGE_STR "Usage: " PROGNAME " <+|->\n"
#define PERMS_STR PROGNAME " must be run with root permissions\n"

/* Configuration */
#define D_STEP 100
#define D_DEV  "intel_backlight"
#define D_PATH "/sys/class/backlight/" D_DEV "/brightness"

void
bl_set (signed);

signed
bl_get (void);

void
bl_modify (char);


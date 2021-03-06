enlighten(1)

# NAME
enlighten - A simple command-line tool to manage sysfs-exposed backlight devices

# SYNOPSIS
*enlighten* [ env | help | list | version | [=+-]<#>[%] ]

# DESCRIPTION
*enlighten* is a command-line tool to manage sysfs-exposed backlights

# OPTIONS
*<no argument>*
	Display current brightness information (formatted: *cur / max (perc)*)

*env*
	Print a list of all environment variables (see *CONFIGURATION*) *enlighten* reads, their currently configured values, and their default values.

*help*
	Print help summary and exit.

*list*
	Print a list of known backlight devices found in the search path.
	The currently-configured device is prefixed with " >> ".

*version*
	Print the version information for *enlighten*.

*[=+-]<#>[%]*
	Adjust the current brightness to *#*.
	(The optional sign before *#* is ignored if it is *=*.)
	If the sign is specified as *+* or *-*, adjust relative to the current brightness.
	If *%* is specified, treat *#* as a percent of the maximum.

# DSL

*enlighten* parses a very simple Domain-Specific Language.
It's formal description is as follows:

```
digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
number ::= <digit> [ <number> ]
action ::= = | + | -
brightness ::= <number> [ % ]
command ::= [ <action> ] <brightness>
```

When _<action>_ is not specified in a command, it is assumed to be _=_.

Put more plainly:

- a _number_ consists of one (or more) of any of the 10 standard decimal Arabic numerals
- an _action_ is one of a literal equals sign, a literal plus sign, or a literal minus sign
- a _brightness_ is a _number_ followed by an optional, literal percent sign
- a _command_ is an optional _action_ followed by a _brightness_

# CONFIGURATION
*enlighten* recognizes several environment variables for runtime configuration:

*BACKLIGHT_DEVICE*
	Holds the (optionally partial) name of the device to query/adjust.
	*enlighten* will use the first valid device found while searching through *BACKLIGHT_SEARCH_PATH* whose name begins with the value stored in this variable.
	If not specified, defaults to *""* (the empty string); this results in the first valid device found being used (regardless of its name).

*BACKLIGHT_SEARCH_PATH*
	Holds a colon-separated list of directories to search for backlight devices.
	When getting or setting the current backlight state for the device specified in *BACKLIGHT_DEVICE*, *enlighten* searches the directories in this variable, in-order looking for the first device to match.
	When listing the devices available, *enlighten* will list each valid candidate device from each directory listed in this variable.
	If not specified, defaults to *"/sys/class/backlight:/sys/class/leds"*.

*BACKLIGHT_THRESHOLD_MIN*
	Holds a brightness value (see *DSL*) representing a saturating minimum brightness.
	If the user-specified value would change the backlight's brightness to be less than this minimum, the brightness will be set to this minimum instead.
	If not specified, defaults to *"0"*.

*BACKLIGHT_THRESHOLD_MAX*
	Holds a brightness value (see *DSL*) representing a saturating maximum brightness.
	If the user-specified value would change the backlight's brightness to be greater than this maximum, the brightness will be set to this maximum instead.
	If not specified, defaults to *"100%"*.

*BACKLIGHT_TRANSITION_STEPS*
	Holds a number value (see *DSL*) representing the maximum number of steps to take between the current brightness and the target brightness.
	*enlighten* will attempt to honor this number of steps as closely as possible without overshooting the target.
	If the user-specified value results in trying to change the brightness by a fractional value per-step, each step will instead change the brightness by a single point.
	If not specified, defaults to *"1"*.

*BACKLIGHT_TRANSITION_PAUSE*
	Holds a number value (see *DSL*) representing the amount of time (in nanoseconds) to pause between steps in a transition.
	If not specified, defaults to *"0"*.

# EXAMPLES

*enlighten h*
	print the help summary.

*enlighten l*
	print a list of known backlight device names.

*enlighten*
	print the current brightness information.

*enlighten +25%*
	increase the brightness by 25 percent of the maximum.

*enlighten 75%*
	set the brightness to 75 percent of the maximum.

# BUGS
Report bugs for enlighten to https://github.com/HalosGhost/enlighten/issues

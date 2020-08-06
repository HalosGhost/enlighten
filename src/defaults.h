#if !defined(DEFAULT_CONFIG_H)
#define DEFAULT_CONFIG_H

#pragma once

#ifndef DEVICE // backlight device name
#define DEVICE "intel_backlight"
#endif

#ifndef SEARCH_PATH // colon-separated pathlist to search for devices
#define SEARCH_PATH "/sys/class/backlight:/sys/class/leds"
#endif

#ifndef THRESHOLD_MAX // maximum brightness threshold
#define THRESHOLD_MAX "100%"
#endif

#ifndef THRESHOLD_MIN // minimum brightness threshold
#define THRESHOLD_MIN "0"
#endif

#ifndef TRANSITION_STEPS // steps to take during a transition
#define TRANSITION_STEPS "1"
#endif

#ifndef TRANSITION_PAUSE // nanoseconds to wait between transition steps
#define TRANSITION_PAUSE "0"
#endif

#endif // DEFAULT_CONFIG_H

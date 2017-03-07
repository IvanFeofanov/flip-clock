#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// debug
#define LED_PIN (13)

// buttons
#define BUTTON_ACTIVE          (false)
#define BUTTON_SELECT_PIN      (2)
#define BUTTON_PLUS_PIN        (4)
#define BUTTON_CANCEL_PIN      (7)

// potentiometr
#define POT_ALARM_CLOCK_PIN    (A0)

// backlight
#define LIGHT_SENSOR_PIN       (A1)
#define BACKLIGHT_HOUR_PIN     (3)
#define BACKLIGHT_MIN_PIN      (5)
#define BACKLIGHT_ALARM_PIN    (6)

#endif

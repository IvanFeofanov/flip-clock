#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// debug
#define LED_PIN (13)

// buttons
#define BUTTON_ACTIVE_LOW      (LOW) // состояние на пине когда кнопка не нажата
#define MODE_BUTTON_PIN        (2)
#define SUPER_BUTTON_PIN       (4)

// potentiometr
#define POT_ALARM_CLOCK_PIN    (A0)  // аналоговый пин к которому подключен потенциометр настройки будильника

// backlight
#define LIGHT_SENSOR_PIN       (A1)
#define BACKLIGHT_HOUR_PIN     (3)
#define BACKLIGHT_MIN_PIN      (5)
#define BACKLIGHT_ALARM_PIN    (6)

#endif

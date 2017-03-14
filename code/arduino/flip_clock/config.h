#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// debug
#define LED_PIN (13)

// buttons
#define BUTTON_ACTIVE_LOW      (LOW) // состояние на пине когда кнопка не нажата
#define MODE_BUTTON_PIN        (2)
#define PLUS_BUTTON_PIN        (4)

// backlight
#define LIGHT_SENSOR_PIN           (A0)
#define LIGHT_SENSOR_DIS_THRES     (512) // значение на датчике освещенности, при котором отключается подсветка
#define LIGHT_SENSOR_EN_THRES      (255)  // значение на датчике освещенности, при котором включается подсветка
#define BACKLIGHT_HOUR_PIN         (3)
#define BACKLIGHT_MINUTE_PIN       (5)

#endif

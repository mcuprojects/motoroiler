#ifndef STRINGS_H
#define STRINGS_H

#include <Arduino.h>
#include <avr/pgmspace.h>

#define LANG_EN 0
#define LANG_RU 1

enum {
  STR_NONE = 0,
  STR_MODE,
  STR_INTERVAL,
  STR_AMMOUNT,
  STR_MIN_SPEED,
  STR_FORCE_PUMP,
  STR_SPEED_TRIM,
  STR_SPEED_K,
  STR_LOW_WARNING,
  STR_UNITS,
  STR_INFO,
  STR_IND_SENSOR,
  STR_EXT_SENSOR,
  STR_K_LINE,
  STR_TIME,
  STR_MANUAL,
  STR_SOLID,
  STR_FLASH,
  STR_KM_C,
  STR_MILES_F,
  STR_HOLD_FOR_INFO,
  STR_INFO_IND_SENSOR,
  STR_INFO_EXT_SENSOR,
  STR_INFO_K_LINE,
  STR_INFO_TIME,
  STR_INFO_MANUAL,
  STR_INFO_SOLID,
  STR_INFO_FLASH
};

typedef uint8_t str_id_t;

const __FlashStringHelper *tr(str_id_t str_id);
extern uint8_t lang;

#endif // STRINGS_H

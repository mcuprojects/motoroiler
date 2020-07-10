#ifndef MENU_ENTRIES_H
#define MENU_ENTRIES_H

#include "Strings.h"

#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

#define ENTRY_TYPE_OTHER 0          // info menu
#define ENTRY_TYPE_CHOICE 1         // choice from multiple options
#define ENTRY_TYPE_INT 2            // integer value
#define ENTRY_TYPE_ON_OFF 3         // on/off switch
#define ENTRY_TYPE_HOLD 4           // hold button to do something

struct choice_item_t {
  const str_id_t name;              // name displayed in second-level menu
  const str_id_t info;              // info displayed by long-press
};

struct menu_entry_t {
  const uint8_t type;               // type (CHOICE/INT/ONOFF)
  bool hidden;                      // option to hide menu entry
  const str_id_t name;              // entry name displayed in top menu
  const choice_item_t *options;     // list of options for CHOICE type
  const size_t options_count;       // size of options list
};

const PROGMEM choice_item_t mode_options[] = {
  { .name = STR_IND_SENSOR,     .info = STR_INFO_IND_SENSOR},
  { .name = STR_EXT_SENSOR,     .info = STR_INFO_EXT_SENSOR},
  { .name = STR_K_LINE,         .info = STR_INFO_K_LINE},
  { .name = STR_TIME,           .info = STR_INFO_TIME},
  { .name = STR_MANUAL,         .info = STR_INFO_MANUAL}
};

const PROGMEM choice_item_t warn_options[] = {
  { .name = STR_SOLID,          .info = STR_INFO_SOLID },
  { .name = STR_FLASH,          .info = STR_INFO_FLASH }
};

const PROGMEM choice_item_t units_options[] = {
  { .name = STR_KM_C,           .info = 0 },
  { .name = STR_MILES_F,        .info = 0 }
};

menu_entry_t menu_entries[] = {
  { .type = ENTRY_TYPE_CHOICE,  .hidden = false, .name = STR_MODE,        .options = mode_options,  .options_count = _countof(mode_options) },
  { .type = ENTRY_TYPE_INT,     .hidden = false, .name = STR_INTERVAL,    .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .hidden = false, .name = STR_AMMOUNT,     .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .hidden = false, .name = STR_MIN_SPEED,   .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_HOLD,    .hidden = false, .name = STR_FORCE_PUMP,  .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .hidden = false, .name = STR_SPEED_K,     .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .hidden = false, .name = STR_SPEED_TRIM,  .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_CHOICE,  .hidden = false, .name = STR_LOW_WARNING, .options = warn_options,  .options_count = _countof(warn_options) },
  { .type = ENTRY_TYPE_CHOICE,  .hidden = false, .name = STR_UNITS,       .options = units_options, .options_count = _countof(units_options) },
  { .type = ENTRY_TYPE_OTHER,   .hidden = false, .name = STR_INFO,        .options = NULL,          .options_count = 0 }
};
const size_t menu_len = _countof(menu_entries);

#endif // MENU_ENTRIES_H

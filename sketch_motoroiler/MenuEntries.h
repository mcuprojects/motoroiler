#ifndef MENU_ENTRIES_H
#define MENU_ENTRIES_H

#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

#define ENTRY_TYPE_OTHER 0          // info menu
#define ENTRY_TYPE_CHOICE 1         // choice from multiple options
#define ENTRY_TYPE_INT 2            // integer value
#define ENTRY_TYPE_ON_OFF 3         // on/off switch
#define ENTRY_TYPE_HOLD 4           // hold button to do something

struct choice_item_t {
  const char *name;                 // name displayed in second-level menu
  const char *info;                 // info displayed by long-press
};

struct menu_entry_t {
  const uint8_t type;               // type (CHOICE/INT/ONOFF)
  uint16_t value;                   // for TYPE_CHOICE: index of currently selected item
                                    // for TYPE_INT: current value
                                    // for TYPE_ON_OFF: 0 = OFF; 1 = ON
  bool hidden;                      // option to hide menu entry
  const char *name;                 // entry name displayed in top menu
  const choice_item_t *options;     // list of options for CHOICE type
  const size_t options_count;       // size of options list
};

const PROGMEM choice_item_t mode_options[] = {
  { .name = "INDUCT SENSOR",  .info = ""},//Chain lubrication based on traveled distance. Measures motorcycle speed from a built-in induction speed sensor" },
  { .name = "EXT SENSOR",     .info = ""},//Chain lubrication based on traveled distance. Measures motorcycle speed from a signal on EXT pin" },
  { .name = "K-LINE",         .info = ""},//Chain lubrication based on traveled distance. Takes motorcycle speed from K-Line" },
  { .name = "TIME",           .info = ""},//Chain lubrication based on fixed time intervals" },
  { .name = "MANUAL",         .info = ""}//Auto-lubrication disabled" }
};

const PROGMEM choice_item_t warn_options[] = {
  { .name = "SOLID",          .info = "When oil level goes low, the warning light lights up" },
  { .name = "FLASH",          .info = "When oil level goes low, the warning light starts to blink" }
};

const PROGMEM choice_item_t units_options[] = {
  { .name = "km/C",           .info = NULL },
  { .name = "miles/F",        .info = NULL }
};

const menu_entry_t menu_entries[] = {
  { .type = ENTRY_TYPE_CHOICE,  .value = 0, .hidden = false, .name = "MODE",        .options = mode_options,  .options_count = _countof(mode_options) },
  { .type = ENTRY_TYPE_INT,     .value = 0, .hidden = false, .name = "INTERVAL",    .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .value = 0, .hidden = false, .name = "AMMOUNT",     .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .value = 0, .hidden = false, .name = "MIN SPEED",   .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_HOLD,    .value = 0, .hidden = false, .name = "FORCE PUMP",  .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .value = 0, .hidden = false, .name = "SPEED K",     .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_INT,     .value = 0, .hidden = false, .name = "SPEED TRIM",  .options = NULL,          .options_count = 0 },
  { .type = ENTRY_TYPE_CHOICE,  .value = 0, .hidden = false, .name = "LOW WARNING", .options = warn_options,  .options_count = _countof(warn_options) },
  { .type = ENTRY_TYPE_CHOICE,  .value = 0, .hidden = false, .name = "UNITS",       .options = units_options, .options_count = _countof(units_options) },
  { .type = ENTRY_TYPE_OTHER,   .value = 0, .hidden = false, .name = "INFO",        .options = NULL,          .options_count = 0 }
};
const size_t menu_len = _countof(menu_entries);

#endif // MENU_ENTRIES_H

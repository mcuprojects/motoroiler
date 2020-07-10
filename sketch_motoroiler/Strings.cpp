#include "Strings.h"

uint8_t lang = LANG_EN;

#define TR(id, string) case id: return F(string)

const __FlashStringHelper *tr(str_id_t str_id) {
  if (lang == LANG_EN)
    switch (str_id)
    {
      TR(STR_MODE, "MODE");
      TR(STR_INTERVAL, "INTERVAL");
      TR(STR_AMMOUNT, "AMMOUNT");
      TR(STR_MIN_SPEED, "MIN SPEED");
      TR(STR_FORCE_PUMP, "FORCE PUMP");
      TR(STR_SPEED_TRIM, "SPEED TRIM");
      TR(STR_SPEED_K, "SPEED K");
      TR(STR_LOW_WARNING, "LOW WARNING");
      TR(STR_UNITS, "UNITS");
      TR(STR_INFO, "INFO");
      TR(STR_IND_SENSOR, "INDUCT SENSOR");
      TR(STR_EXT_SENSOR, "EXT SENSOR");
      TR(STR_K_LINE, "K-LINE");
      TR(STR_TIME, "TIME");
      TR(STR_MANUAL, "MANUAL");
      TR(STR_SOLID, "SOLID");
      TR(STR_FLASH, "FLASH");
      TR(STR_KM_C, "km/C");
      TR(STR_MILES_F, "miles/F");
      TR(STR_HOLD_FOR_INFO, "Hold \x07 for info");
      TR(STR_INFO_IND_SENSOR, "Chain lubrication based on traveled distance. Measures motorcycle speed from a built-in induction speed sensor");
      TR(STR_INFO_EXT_SENSOR, "Chain lubrication based on traveled distance. Measures motorcycle speed from a signal on EXT pin");
      TR(STR_INFO_K_LINE, "Chain lubrication based on traveled distance. Takes motorcycle speed from K-Line");
      TR(STR_INFO_TIME, "Chain lubrication based on fixed time intervals");
      TR(STR_INFO_MANUAL, "Auto-lubrication disabled");
      TR(STR_INFO_SOLID, "When oil level goes low, the warning light lights up");
      TR(STR_INFO_FLASH, "When oil level goes low, the warning light starts to blink");
      default:
        return F("");
    }
  return F("");
}

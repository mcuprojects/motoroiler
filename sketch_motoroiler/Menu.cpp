#include "Menu.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "hardwareSerial.h"
#include "globalTimer.h"
#include "MenuEntries.h"

#define UP_BMP_HEIGHT   4
#define UP_BMP_WIDTH    8
static const unsigned char PROGMEM up_bmp[] =
{ B00010000,
  B00111000,
  B01111100,
  B11111110};

#define DOWN_BMP_HEIGHT   4
#define DOWN_BMP_WIDTH    8
static const unsigned char PROGMEM down_bmp[] =
{ B11111110,
  B01111100,
  B00111000,
  B00010000};

#define OK_BMP_HEIGHT   8
#define OK_BMP_WIDTH    8
static const unsigned char PROGMEM ok_bmp[] =
{ B00111000,
  B01111100, 
  B11111110,
  B11111110,
  B11111110,
  B01111100,
  B00111000,
  B00000000}; 

#define SMALL_CHAR_WIDTH 6
#define LARGE_CHAR_WIDTH 12
#define SCROLL_PRE_DELAY 1000
#define SCROLL_STEP_DELAY 15
#define SCROLL_POST_DELAY 1000

#define TOP_BAR_HEIGHT 9
#define SIDE_BAR_WIDTH 9

#define SCROLL_OFF    0
#define SCROLL_START  1
#define SCROLL_MOVE   2
#define SCROLL_END    3

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint16_t scroll_pos = 0;
uint16_t scroll_count = 0;
str_id_t scroll_text = 0;
str_id_t menu_header_text = 0;
void (*scroll_draw_func)(const char *text, const uint16_t offset) = NULL;
volatile uint8_t scroll_state = SCROLL_OFF;

uint8_t top_menu_pos = 0;

// INTERNAL FUNCTIONS

void draw_middle_menu_line(const str_id_t text, const uint16_t offset = 0) {
  display.fillRect(0, 7, display.width(), 18, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(2);
  display.setCursor(2 - offset, 9);
  display.setTextWrap(false);
  display.println(tr(text));

  display.display();
}

void draw_sub_menu_sidebar(bool up, bool down) {
  display.fillRect(display.width() - SIDE_BAR_WIDTH, TOP_BAR_HEIGHT, SIDE_BAR_WIDTH, display.height() - TOP_BAR_HEIGHT, SSD1306_BLACK);
  if (up)
    display.drawBitmap(display.width() - SIDE_BAR_WIDTH + 1, 15, up_bmp, UP_BMP_WIDTH, UP_BMP_HEIGHT, SSD1306_WHITE);
  if (down)
    display.drawBitmap(display.width() - SIDE_BAR_WIDTH + 1, 23, down_bmp, DOWN_BMP_WIDTH, DOWN_BMP_HEIGHT, SSD1306_WHITE);
}

void draw_sub_menu_line(const str_id_t text, const uint16_t offset = 0) {
  display.fillRect(0, TOP_BAR_HEIGHT, display.width(), display.height() - TOP_BAR_HEIGHT, SSD1306_BLACK);
  // draw value
  display.setTextSize(2);
  display.setCursor(0 - offset, 14);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.println(tr(text));
  // draw side arrows
  draw_sub_menu_sidebar();

  display.display();
}

void draw_info(const str_id_t info_text) {
  // draw info header
  char buffer[20];
  strcpy_P(buffer, (char*)tr(info_text));
  buffer[19] = 0;
  display.fillRect(0, 0, display.width(), TOP_BAR_HEIGHT, SSD1306_WHITE);
  uint16_t w = SMALL_CHAR_WIDTH * strlen(buffer);
  uint16_t x = display.width() - w - 1;
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(x, 1);
  display.println(buffer);
  char *pos = strchr(buffer, '\x07');
  if (pos) {
    x += (pos - buffer) * SMALL_CHAR_WIDTH;
    display.drawBitmap(x, 1, ok_bmp, OK_BMP_WIDTH, OK_BMP_HEIGHT, SSD1306_BLACK);
  }
}

void hide_info() {
  // draw regular header
  display.fillRect(0, 0, display.width(), TOP_BAR_HEIGHT, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(1, 1);
  display.println(tr(menu_header_text));
  display.display();
}

void write_scrolling_text(const str_id_t text, void(*text_draw_func)(const str_id_t text, const uint16_t offset), const uint16_t width) {
  const uint16_t w_text = LARGE_CHAR_WIDTH * strlen(text);
  scroll_count = 0;
  scroll_text = text;
  scroll_draw_func = text_draw_func;
  scroll_state = SCROLL_OFF;
  text_draw_func(text, 0);
  if (w_text > width) {
    scroll_count = w_text - width + 1;
    scroll_state = SCROLL_START;
    timer.in(SCROLL_PRE_DELAY, text_scrolling_loop);
  }
}

void text_scrolling_loop() {
  switch (scroll_state) {
  case SCROLL_START:
    scroll_pos = 0;
    scroll_state = SCROLL_MOVE;
    timer.in(SCROLL_STEP_DELAY, text_scrolling_loop);
    break;
  case SCROLL_MOVE:
    scroll_pos++;
    if (scroll_pos < scroll_count) {
      scroll_draw_func(scroll_text, scroll_pos);
      timer.in(SCROLL_STEP_DELAY, text_scrolling_loop);
    } else {
      scroll_state = SCROLL_END;
      timer.in(SCROLL_POST_DELAY, text_scrolling_loop);
    }
    break;
  case SCROLL_END:
    scroll_draw_func(scroll_text, 0);
    scroll_state = SCROLL_START;
    timer.in(SCROLL_PRE_DELAY, text_scrolling_loop);
    break;
  default:
    break;
  }
}

void draw_desc_menu() {
  display.clearDisplay();
  display.setTextSize(1);

  const char PROGMEM menu_name[] = "MODE";
  const char PROGMEM menu_value[] = "MANUAL 1";
  const char PROGMEM menu_description[] = "Oil is dispensed with external button.";
  const uint16_t DESC_LINE_SIZE = 21;

  // draw header
  display.fillRect(0, 0, display.width(), 9, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(1, 1);
  display.println(menu_name);

  // draw value
  uint16_t w = 30;
  display.getTextBounds(menu_value, 0, 0, NULL, NULL, &w, NULL);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(display.width() - w - 1, 1);
  display.println(menu_value);

  // draw description
  display.setTextColor(SSD1306_WHITE);
  if (strlen(menu_description) <= DESC_LINE_SIZE)
    display.setCursor(0, 16);
  else if (strlen(menu_description) <= DESC_LINE_SIZE * 2)
    display.setCursor(0, 12);
  else
    display.setCursor(0, 9);
  display.println(menu_description);

  display.display();
}

int8_t prev_menu_entry(int index)
{
  for (int i = index - 1; i >= 0; i--)
    if (!menu_entries[i].hidden)
      return i;
  return -1;  // not found
}

int8_t next_menu_entry(int index)
{
  for (int i = index + 1; i < menu_len; i++)
    if (!menu_entries[i].hidden)
      return i;
  return -1;  // not found
}

// PUBLIC FUNCTIONS

void init_menu() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
}

void clear_screen() {
  display.fillRect(0, 0, display.width(), display.height(), SSD1306_BLACK);
  display.display();
}

void draw_top_menu(uint8_t index) {
  display.clearDisplay();
  display.setTextSize(2);
  
  // draw upper line
  int8_t prev = prev_menu_entry(index);
  if (prev >= 0) {
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, -9);
    display.println(tr(menu_entries[prev].name));
  }
  
  // draw bottom line
  int8_t next = next_menu_entry(index);
  if (next >= 0) {
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 27);
    display.println(tr(menu_entries[next].name));
  }

  // draw middle line (scrolling)
  write_scrolling_text(menu_entries[index].name, &draw_middle_menu_line, display.width() - 2);
}

void draw_sub_menu(uint8_t index, uint8_t option) {
  display.clearDisplay();
  display.setTextSize(1);

  menu_header_text = menu_entries[index].name;
  if (menu_entries[index].options[option].info != 0) {
    // display info hint
    draw_info(STR_HOLD_FOR_INFO);
    timer.in(1500, hide_info);
  }
  else {
    // display header text
    hide_info();
  }

  // display option
  write_scrolling_text(menu_entries[index].options[option].name, &draw_sub_menu_line, display.width() - 2 - SIDE_BAR_WIDTH);
}

void menu_up()
{
  if (top_menu_pos > 0)
    top_menu_pos--;

  draw_top_menu(top_menu_pos);
}

void menu_down()
{
  if (top_menu_pos < (menu_len - 1))
    top_menu_pos++;

  draw_top_menu(top_menu_pos);
}

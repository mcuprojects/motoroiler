#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <timer.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// arduino-timer library
auto timer = timer_create_default();

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
#define SCROLL_PRE_DELAY 800
#define SCROLL_STEP_DELAY 12
#define SCROLL_POST_DELAY 800

#define TOP_BAR_HEIGHT 9
#define SIDE_BAR_WIDTH 9

#define SCROLL_OFF    0
#define SCROLL_START  1
#define SCROLL_MOVE   2
#define SCROLL_END    3
uint16_t scroll_pos = 0;
uint16_t scroll_count = 0;
char *scroll_text = NULL;
void(*scroll_draw_func)(const char *text, const uint16_t offset) = NULL;
volatile uint8_t scroll_state = SCROLL_OFF;

//Timer scrollTimer(true, &text_scrolling_loop);
//Timer infoTimer(true, &hide_info);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  //ArduTimer::setup(ArduTimer::LOOP);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);

  //draw_top_menu();
  //delay(1000);
  draw_edit_menu();
  //delay(1000);
}

void loop() {
  //ArduTimer::loop();
  timer.tick();
}

void draw_sidebar() {
  const uint8_t x = display.width() - SIDE_BAR_WIDTH;
  display.fillRect(x, 0, 9, display.height(), SSD1306_WHITE);
  display.drawLine(x - 1, 0, x - 1, display.height(), SSD1306_BLACK);
  display.drawBitmap(x + 1, 2, up_bmp, UP_BMP_WIDTH, UP_BMP_HEIGHT, SSD1306_BLACK);
  display.drawBitmap(x + 1, 13, ok_bmp, OK_BMP_WIDTH, OK_BMP_HEIGHT, SSD1306_BLACK);
  display.drawBitmap(x + 1, 27, down_bmp, DOWN_BMP_WIDTH, DOWN_BMP_HEIGHT, SSD1306_BLACK);
}

void draw_middle_menu_line(const char *text, const uint16_t offset = 0) {
  display.fillRect(0, 7, display.width(), 18, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(2);
  display.setCursor(2 - offset, 9);
  display.setTextWrap(false);
  display.println(text);

  display.display();
}

void draw_top_menu() {
  display.clearDisplay();
  display.setTextSize(2);
  
  // draw upper line
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, -9);
  display.println(F(""));
  
  // draw bottom line
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 27);
  display.println(F("MODE"));

  // draw middle line (scrolling)
  write_scrolling_text("LONG SCROLLING TEXT", &draw_middle_menu_line, display.width() - 2);
}

void draw_edit_menu_line(const char *text, const uint16_t offset = 0) {
  display.fillRect(0, TOP_BAR_HEIGHT, display.width(), display.height() - TOP_BAR_HEIGHT, SSD1306_BLACK);
  // draw value
  display.setTextSize(2);
  display.setCursor(0 - offset, 14);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.println(text);

  // draw sidebar
  display.fillRect(display.width() - SIDE_BAR_WIDTH, TOP_BAR_HEIGHT, SIDE_BAR_WIDTH, display.height() - TOP_BAR_HEIGHT, SSD1306_BLACK);
  display.drawBitmap(display.width() - SIDE_BAR_WIDTH + 1, 15, up_bmp, UP_BMP_WIDTH, UP_BMP_HEIGHT, SSD1306_WHITE);//EdaDenuz
  display.drawBitmap(display.width() - SIDE_BAR_WIDTH + 1, 23, down_bmp, DOWN_BMP_WIDTH, DOWN_BMP_HEIGHT, SSD1306_WHITE);

  display.display();
}

void draw_edit_menu() {
  display.clearDisplay();
  display.setTextSize(1);

  const char PROGMEM menu_name[] = "MODE";
  const char PROGMEM menu_value[] = "MANUAL MODE 1";
  const char PROGMEM info_text[] = "Hold \x07 for info";
  const uint16_t CHAR_WIDTH = 6;

  draw_info(info_text);
  timer.in(1500, hide_info);

  write_scrolling_text(menu_value, &draw_edit_menu_line, display.width() - 2 - SIDE_BAR_WIDTH);
}

void draw_info(const char *info_text) {
  // draw info header
  display.fillRect(0, 0, display.width(), TOP_BAR_HEIGHT, SSD1306_WHITE);
  uint16_t w = SMALL_CHAR_WIDTH * strlen(info_text);
  uint16_t x = display.width() - w - 1;
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(x, 1);
  display.println(info_text);
  char *pos = strchr(info_text, '\x07');
  if (pos) {
    x += (pos - info_text) * SMALL_CHAR_WIDTH;
    display.drawBitmap(x, 1, ok_bmp, OK_BMP_WIDTH, OK_BMP_HEIGHT, SSD1306_BLACK);
  }
}

void hide_info() {
  // draw regular header
  display.fillRect(0, 0, display.width(), TOP_BAR_HEIGHT, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(1, 1);
  display.println("MODE");
}

void write_scrolling_text(const char *text, void(*text_draw_func)(const char *text, const uint16_t offset), const uint16_t width) {
  const uint16_t w_text = LARGE_CHAR_WIDTH * strlen(text);
  scroll_count = 0;
  scroll_text = text;
  scroll_draw_func = text_draw_func;
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
    scroll_draw_func(scroll_text, scroll_pos);
    if (scroll_pos < scroll_count) {
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

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

void init_menu();
void draw_top_menu(uint8_t index = 0);
void draw_sub_menu(uint8_t index = 0, uint8_t option = 0);

void draw_middle_menu_line(const char *text, const uint16_t offset = 0);
void draw_sub_menu_sidebar(bool up = true, bool down = true);
void draw_sub_menu_line(const char *text, const uint16_t offset = 0);
void draw_info(const char *info_text);
void hide_info();
void write_scrolling_text(const char *text, void(*text_draw_func)(const char *text, const uint16_t offset), const uint16_t width);
void text_scrolling_loop();
void draw_desc_menu();

#endif // MENU_H

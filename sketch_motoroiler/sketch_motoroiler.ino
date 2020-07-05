#include "globalTimer.h"
#include "Interface.h"
#include "hardwareSerial.h"

void setup() {
  Serial.begin(9600);

  init_menu();
  draw_top_menu(0);
  delay(400);
  draw_top_menu(1);
  delay(400);
  draw_top_menu(2);
  delay(400);
  draw_top_menu(3);
  delay(400);
  draw_top_menu(4);
  delay(400);
  draw_top_menu(5);
  delay(400);
  draw_top_menu(6);
  delay(400);
  draw_top_menu(7);
  delay(400);
  draw_top_menu(8);
  delay(400);
  draw_top_menu(9);
  delay(400);
  draw_sub_menu(0, 2);
}

void loop() {
  // timer events
  timer.tick();

  // check button inputs
}

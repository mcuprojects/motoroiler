#include "globalTimer.h"
#include "Interface.h"
#include "hardwareSerial.h"

uint8_t btn_state = 0;

void setup() {
  Serial.begin(9600);

  oled_on();
  
  pinMode(7, OUTPUT);
  //digitalWrite(7, 1);
  pinMode(9, OUTPUT);
  //digitalWrite(9, 1);

  init_menu();
  draw_top_menu(0);

  //oled_off();
}

void loop() {
  // timer events
  timer.tick();

  // check buttons
  if (digitalRead(2) == 0) {  // UP
    if (btn_state == 0)
      menu_up();
    btn_state = 1;
  }
  else if (digitalRead(4) == 0) {  // DOWN
    if (btn_state == 0)
      menu_down();
    btn_state = 2;
  }
  else
    btn_state = 0;
}

void oled_on()
{
  pinMode(A2, OUTPUT);
  digitalWrite(A2, 0);
}

void oled_off()
{
  clear_screen();
  pinMode(A2, INPUT);
  digitalWrite(A2, 1);
}

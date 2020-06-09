#include "ArduTimer.h"
#include "avr/interrupt.h"
#include "hardwareSerial.h"

#define TIMER_FLAG_IRQ    0x01
#define TIMER_FLAG_SS     0x02
#define TIMER_FLAG_EN     0x04

#ifndef F_CPU
#define F_CPU 16000000
#endif

#define F_TIMER 1000
#define F_PRESC 64

struct timer_entry {
  Timer *tm;
  timer_entry *next;
};

ArduTimer::Mode ArduTimer::_mode = ArduTimer::LOOP;
static timer_entry *timer_list = NULL;

ISR(TIMER2_COMPA_vect) {
  ArduTimer::_timer_tick();
}

Timer::Timer(bool singleShot = true, void(*callback)(void) = 0) :
  _status_reg(0x00),
  _value(0),
  _reset_value(0),
  _callback(callback)
{
  if (singleShot)
    _status_reg |= TIMER_FLAG_SS;
  ArduTimer::_register_timer(this);
}

void Timer::start(uint32_t time_ms)
{
  _reset_value = time_ms;
  _value = time_ms;
  _status_reg |= TIMER_FLAG_EN;
}

void Timer::pause()
{
  _status_reg &= ~TIMER_FLAG_EN;
}

void Timer::resume()
{
  _status_reg |= TIMER_FLAG_EN;
}

void Timer::stop()
{
  _status_reg &= ~TIMER_FLAG_EN;
}

bool Timer::isRunning()
{
  return (_status_reg & TIMER_FLAG_EN) != 0;
}

void Timer::setSingleShot(bool singleShot)
{
  if (singleShot)
    _status_reg |= TIMER_FLAG_SS;
  else
    _status_reg &= ~TIMER_FLAG_SS;
}
void Timer::setCallback(void(*callback)(void))
{
  _callback = callback;
}

void Timer::clearCallback()
{
  _callback = NULL;
}

void ArduTimer::_register_timer(Timer *timer)
{
  timer_entry *last = timer_list;
  timer_entry *prev = NULL;

  // find list tail
  while (last != NULL) {
    prev = last;
    last = last->next;
  }

  // allocate timer list entry
  last = malloc(sizeof(timer_entry));
  // check if allocation failed
  if (last == NULL)
    //Serial.println(F("ArduTimer ERROR: Failed to register Timer object.\r\n"
    //                 "Probably you are using too much timers and run out of RAM.\r\n"
    //                 "\r\n"));
    return;

  // append timer to list
  last->tm = timer;
  last->next = NULL;
  if (prev != NULL)
    prev->next = last;
  else
    timer_list = last;
}

void ArduTimer::_unregister_timer(Timer *timer)
{
  timer_entry *p = timer_list;
  timer_entry *prev = NULL;
  
  // iterate registereded timers
  while (p != NULL) {
    if (p->tm == timer) {
      if (prev != NULL)
        prev->next = p->next;
      else
        timer_list = p->next;
      free(p);
    }
  }
}

void ArduTimer::_timer_tick()
{
  timer_entry *p = timer_list;
  
  // iterate registereded timers
  while (p != NULL) {
    Timer *timer = p->tm;
    if (timer->_status_reg & TIMER_FLAG_EN) {
      if (timer->_value)
        timer->_value--;
      if (timer->_value == 0) {
        // restart timer if not singleshot
        if (timer->_status_reg & TIMER_FLAG_SS)
          timer->_status_reg &= ~TIMER_FLAG_EN;
        else
          timer->_value = timer->_reset_value;

        // generate timer event
        if (_mode == IRQ) {
          // call callback immediately in IRQ mode
          if (timer->_callback)
            timer->_callback();
        } else if (_mode == LOOP) {
          // set IRQ flag in LOOP mode
          timer->_status_reg |= TIMER_FLAG_IRQ;
        }
      }
    }
    p = p->next;
  }
}

void ArduTimer::setup(Mode mode = LOOP, uint8_t hwTimer = 2)
{
  _mode = mode;

  TCCR2B = (1<<WGM21) | (0<<WGM20);
  TCCR2B = (1<<CS22) | (0<<CS21) | (0<<CS20); // CLK / 128 = 125 kHz
  OCR2A = 125;
  TIMSK2 = (1<<OCIE2A);
  sei();
}

void ArduTimer::loop()
{
  timer_entry *p = timer_list;

  if (_mode != LOOP)
    //Serial.println(F("ArduTimer WARNING: you are usnig ArduTimer in IRQ mode and calling ArduTimer::loop() function.\r\n"
    //                 "loop() is not supposed to be called in IRQ mode.\r\n"
    //                 "Prefered mode of running is LOOP mode, use IRQ mode only if you know what you're doing.\r\n"
    //                 "\r\n"));
    return;
  
  // iterate registereded timers
  while (p != NULL) {
    Timer *timer = p->tm;
    if (timer->_status_reg & TIMER_FLAG_IRQ) {
      timer->_status_reg &= ~TIMER_FLAG_IRQ;
      if (timer->_callback)
        timer->_callback();
    }
    p = p->next;
  }
}

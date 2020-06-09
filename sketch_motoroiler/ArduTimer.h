#ifndef ARDUTIMER_H
#define ARDUTIMER_H

#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"

// Forward declarations
class Timer;
class ArduTimer;

class Timer
{
  public:
    Timer(bool singleShot, void(*callback)(void) = 0);
    void start(uint32_t time_ms);
    bool isRunning();
    void pause();
    void resume();
    void stop();
    void setSingleShot(bool singleShot);
    void setCallback(void(*callback)(void));
    void clearCallback();

    volatile uint8_t _status_reg;
    volatile uint32_t _value;
    volatile uint32_t _reset_value;
    volatile void(*_callback)(void);
};

class ArduTimer
{
  public:
    enum Mode {
      IRQ,
      LOOP
    };
    
    static void setup(Mode mode = LOOP, uint8_t hwTimer = 2);
    static void loop();
    
    static Mode _mode;
    static void _timer_tick();
    static void _register_timer(Timer *timer);
    static void _unregister_timer(Timer *timer);
};

#endif // ARDUTIMER_H

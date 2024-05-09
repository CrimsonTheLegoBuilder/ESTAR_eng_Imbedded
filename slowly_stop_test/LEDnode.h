#ifndef _LEDNODE_H_
#define _LEDNODE_H_

#define NORMAL 1
#define DECREASE 2
#define STOP 3

#include <Arduino.h>

#define RED 7
#define detector 5

namespace LED {
  bool init();
  bool write(const int& brt);
  bool led_off(const int& brt);
  bool slowly_down(int& brt);
  bool run_led(const int& mode, int& brt = 0);
  bool detect_disturbance();
}

#endif
#ifndef _LEDNODE_H_
#define _LEDNODE_H_

#define NORMAL 1
#define DECREASE -1
#define STOP 0

#include <Arduino.h>

#define RED 7
#define detector 5

namespace LED {
  bool init();
  bool detect_disturbance();
  int detect_disturbances(int& prev);
  int write(const int& brt);
  int led_off(const int& brt);
  int slowly_down(int& brt);
  int run_led(const int& mode, const int& door_state, int& brt = 0);
}

#endif
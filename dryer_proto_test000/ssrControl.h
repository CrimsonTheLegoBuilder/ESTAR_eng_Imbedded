#ifndef _SSR_H_
#define _SSR_H_

#include <Arduino.h>
#include <stdio.h>
#include <math.h>

#define NORMAL 1
#define DECREASE -1
#define STOP 0

#define detector 17

typedef long long ll;
typedef double ld;

namespace ssr {
  inline int init(int heatpump_, int ptc_, int fan_);
  inline bool ssr::detect_disturbance();
  inline int ssr::detect_door(int& prev);
  inline int ptc_control(int w);
  inline int heatpump_control(int w);
  inline int fan_control(int w);zz
  inline int turn_on(int w);
  inline int turn_off();
  inline int run_dryer(bool BUTTON_STATE, bool changed, int w);
}

#endif
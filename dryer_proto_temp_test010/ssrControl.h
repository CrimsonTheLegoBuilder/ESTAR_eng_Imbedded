#ifndef _SSR_H_
#define _SSR_H_

#include <Arduino.h>
#include <stdio.h>
#include <math.h>

typedef long long ll;
typedef double ld;

namespace ssr {
  void init(int heatpump_, int ptc_, int fan_);
  void ptc_control(int w);
  void heatpump_control(int w);
  void fan_control(int w);
  void turn_on(int w);
  void turn_off();
  void run_dryer(bool BUTTON_STATE, bool changed, int w);
}

#endif
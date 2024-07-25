#ifndef _SSR_H_
#define _SSR_H_

#include <Arduino.h>
#include <stdio.h>
#include <math.h>
#include "datainfo.h"

typedef long long ll;
typedef double ld;

namespace ssr {
  void init();
  void ir_control(int w);
  void heatpump_control(int w);
  void fan_control(int w);
  void turn_on(int w);
  void turn_off();
  void run_dryer(bool run_state);
}

#endif
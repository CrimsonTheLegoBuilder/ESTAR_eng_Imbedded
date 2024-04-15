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
}

#endif
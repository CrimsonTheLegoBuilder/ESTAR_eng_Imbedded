#ifndef _SSR_H_
#define _SSR_H_

#include <Arduino.h>
#include "_config.h" 

namespace ssr {
  void init(int heatpump_, int ptc_, int fan_);
  void ptc_control(int w);
  void heatpump_control(int w);
  void fan_control(int w);
}

#endif
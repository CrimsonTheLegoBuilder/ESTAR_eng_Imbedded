#ifndef _WTS_CONTROL_H_
#define _WTS_CONTROL_H_

/*
library for wts3535

*/

#include <Arduino.h>
#include "datainfo.h"

struct wts {
  int t_pin, h_pin;
  wts(int t = 0, int h = 0) : t_pin(t), h_pin(h) {}
  int init(Data& data);
  int measure(Data& data);
  int check(Data& data);
};

// namespace wts {
//   int init(Data& data);
//   int measure(Data& data);
//   int check(Data& data);
// };

#endif
#ifndef _WTS_CONTROL_H_
#define _WTS_CONTROL_H_

/*
library for wts3535

*/

#include <Arduino.h>
#include "datainfo.h"

// #define WTS_PIN 3

// struct Data {
//   float humi, temp;
//   Data(float h = 0, float t = 0) : humi(h), temp(t) {}
// };

namespace wts {
  int init(Data& data);
  int measure(Data& data);
  int check(Data& data);
};

#endif
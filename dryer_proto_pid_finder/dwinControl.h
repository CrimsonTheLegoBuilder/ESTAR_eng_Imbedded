#ifndef _DWIN_CONTROL_H_
#define _DWIN_CONTROL_H_

#include <Arduino.h>
// #include 

#define temperature_add 0x61
#define humidity_add 0x62

typedef long long ll;
typedef double ld;

// struct Data {
//   float humi, temp;
//   Data(float h = 0, float t = 0) : humi(h), temp(t) {}
// };

namespace dwin {
  void setup();
  bool read_setpoint(int& temp);
  void print_humitemp(float temp, float humi);
}

#endif
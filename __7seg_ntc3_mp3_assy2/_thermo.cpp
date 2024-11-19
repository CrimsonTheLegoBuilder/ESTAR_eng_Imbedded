#ifndef __THERMO_SOURCE__
#define __THERMO_SOURCE__

#include "_thermo.h"

float Thermo::cal_thermo(int v) {
  float R = R25 * (1023.0 / (float)v - 1.0);
  float logR = log(R);
  float T = (1.0 / (c1 + c2*logR + c3*logR*logR*logR));
  T = T - 273.15;
  return T;
}
float Thermo::read_temper(int n) {
  int x = n == 1 ? b : n == 2 ? r : w;
  int v = analogRead(x);
  return cal_thermo(v);
}

#endif
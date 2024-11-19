#ifndef __THERMO_H__
#define __THERMO_H__

#include "Arduino.h"

const float R25 = 10000;
const float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

struct Thermo {
  int b, r, w;
  Thermo(int n0 = 0, int n1 = 0, int n2 = 0) {
    pinMode(b = n0, INPUT);
    pinMode(r = n1, INPUT);
    pinMode(w = n2, INPUT);
  }
  float cal_thermo(int v);
  float read_temper(int n);
};

#endif
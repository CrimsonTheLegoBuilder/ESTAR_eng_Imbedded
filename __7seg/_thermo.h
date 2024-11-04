#ifndef __THERMO_H__
#define __THERMO_H__

#include "Arduino.h"

struct Thermo {
  int n;
  Thermo(int n_ = 0) {
    pinMode(n, INPUT);
  }
  float temper();
};

#endif
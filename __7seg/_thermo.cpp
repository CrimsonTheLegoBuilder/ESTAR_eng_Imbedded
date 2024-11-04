#ifndef __THERMO_SOURCE__
#define __THERMO_SOURCE__

#include "_thermo.h"

float Thermo::temper() {
  int v = analogRead(n);
  return v;
}

#endif
#ifndef _SPRAY_SOURCE_
#define _SPRAY_SOURCE_

#include "systemConfig.h"

ld norm(ld t) {
  while (t > 2 * PI - TOL) t -= 2 * PI;
  while (t < 0) t += 2 * PI;
  return t;
}
float norm(float t) {
  while (t > 2 * PI - TOL) t -= 2 * PI;
  while (t < 0) t += 2 * PI;
  return t;
}

#endif
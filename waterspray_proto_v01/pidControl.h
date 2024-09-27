#ifndef _PID_H_
#define _PID_H_

#include <Arduino.h>
#include <stdio.h>
#include "systemConfig.h"

typedef long long ll;

class PID {
public:
  struct gain {
    ld Kp, Ki, Kd, dt, low_limit, high_limit;
    gain(ld Ku = 0, ld Tu = 0, ld dt = 0, ld lo = 0, ld hi = 0) : 
    dt(dt), low_limit(lo), high_limit(hi) {
      Kp = 0.60 * Ku;
      Ki = 0;
      Kd = 0;
      // Ki = 2.0 * Kp / Tu;
      // Kd = Kp * Tu / 8.0;  
    }
  };

  gain params;
  ld integral;
  ld prev_err;
  ld derivative;

  PID(ld ku = 0, ld tu = 0, ld dt = 1e-4, ld lo = 0, ld hi = 0);
  void init(ld ku = 0, ld tu = 0, ld dt = 1e-4, ld lo = 0, ld hi = 0);
  ld pid_converter(ld output);
  ld pid_control(ld setpoint, ld pv);
  void DEBUG_();
};

#endif
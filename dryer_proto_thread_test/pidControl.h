#ifndef _PID_H_
#define _PID_H_

#include <Arduino.h>
#include "_config.h"

ld heat(ld x[], ld t, ld Q);

class PID {
public:
  struct gain {
    ld Kp, Ki, Kd, dt, low_limit, high_limit;
    gain(ld kp = 0, ld ki = 0, ld kd = 0, ld dt = 0, ld lo = 0, ld hi = 0) : 
    Kp(kp), Ki(ki), Kd(kd), dt(dt), low_limit(lo), high_limit(hi) {}
  };

  gain params;
  ld integral;
  ld prev_err;
  ld derivative;

  PID(ld kp, ld ki, ld kd, ld dt, ld lo, ld hi = 70);
  ld pid_converter(ld output);
  ld pid_control(ld setpoint, ld pv);
  void DEBUG();

  //~PID();
};

#endif
#ifndef _PID_H_
#define _PID_H_

#include <Arduino.h>
#include <stdio.h>
#include <math.h>
#include "datainfo.h"

typedef long long ll;
typedef double ld;

ld heat(ld x[], ld t, ld Q);

class PID {
public:
  struct gain {
    ld Kp, Ki, Kd, dt, low_limit, high_limit;
    gain(ld kp = 0, ld ki = 0, ld kd = 0, ld dt = 0, ld lo = 0, ld hi = 0) : 
    Kp(kp), Ki(ki), Kd(kd), dt(dt), low_limit(lo), high_limit(hi) {}
  };

  bool hys_con;
  ld hys;
  ld setpoint;
  gain params;
  ld integral;
  ld prev_err;
  ld derivative;

  PID(int hc = 0, ld h = 0, ld kp = 2., ld ki = .1, ld kd = .01, ld dt = 1e-4, ld lo = 20., ld hi = 70.);
  void init(ld kp = 2., ld ki = .1, ld kd = .01, ld dt = 1e-4, ld lo = 20., ld hi = 70.);
  ld pid_control(ld pv);
  HeaterOrder pid_converter(ld output);
  void compute(ld pv, HeaterOrder& ho);
  void DEBUG();

  //~PID();
};

#endif
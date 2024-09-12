#ifndef _PID_SOURCE_
#define _PID_SOURCE_

#include "pidControl.h"

PID::PID(ld kp, ld ki, ld kd, ld dt, ld lo, ld hi) : params(kp, ki, kd, dt, lo, hi) {
  integral = 0;
  prev_err = 0;
  derivative = 0;
}

void PID::init(ld kp, ld ki, ld kd, ld dt, ld lo, ld hi) {
  params = gain(kp, ki, kd, dt, lo, hi);
  integral = 0;
  prev_err = 0;
  derivative = 0;
}

ld PID::pid_converter(ld output) {
  ld Coef = 1.0;
  return output * Coef;
}

ld PID::pid_control(ld setpoint, ld pv) {
  ld& Kp = params.Kp, Ki = params.Ki, Kd = params.Kd;
  ld& dt = params.dt, lo = params.low_limit, hi = params.high_limit;

  ld err = setpoint - pv;
  Serial.print("err : ");
  Serial.println((float)err);
  Serial.print("prev_err : ");
  Serial.println((float)prev_err);
  integral += err * dt;
  derivative = (err - prev_err) / dt;
  // derivative = (pv - prev_pv) / dt;

  ld Pout = Kp * err;//proportional term
  ld Iout = Ki * integral;//integral term
  ld Dout = Kd * derivative;//derivative term
  ld output = Pout + Iout + Dout;//total output
  // Serial.print("P : ");
  // Serial.print(Pout);
  // Serial.print(" I : ");
  // Serial.print(Iout);
  Serial.print("set point : ");
  Serial.println((float)setpoint);
  // Serial.print("output : ");
  // Serial.println(output);
  //restrict to max/min
  if (output > hi) output = hi;
  else if (output < lo) output = lo;
  Serial.print("output : ");
  Serial.println((float)output);
  prev_err = err;//save error to previous error

  return pid_converter(output);
}

void PID::DEBUG() {
  Serial.print("Debug:: Kp: ");
  Serial.print(params.Kp);
  Serial.print(" Ki: ");
  Serial.print(params.Ki);
  Serial.print(" Kd: ");
  Serial.print(params.Kd);
  Serial.print(" dt: ");
  Serial.print(params.dt);
  Serial.print(" lo: ");
  Serial.print(params.low_limit);
  Serial.print(" hi: ");
  Serial.print(params.high_limit);
  Serial.print(" integ: ");
  Serial.print(integral);
  Serial.print(" prev_err: ");
  Serial.println(prev_err);
  return;
}

#endif
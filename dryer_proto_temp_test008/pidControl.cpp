#ifndef _PID_SOURCE_
#define _PID_SOURCE_

#include "pidControl.h"

ld heat(ld x[], ld t, ld Q) {
    ld Ta = 23 + 273.15;           //K
    ld U = 10.0;                   //W/m^2-K
    ld m = 4.0 / 1000.0;           //kg
    ld Cp = 0.5 * 1000.0;          //J/kg-K    
    ld A = 12.0 / (100.0 * 100.0); //Area in m^2
    ld alpha = 0.01;               //W / % heater
    ld eps = 0.9;                  //Emissivity
    ld sigma = 5.67e-8;            //Stefan-Boltzman
    
    //Temperature State 
    ld T = x[0];
    ld TA = Ta * Ta * Ta * Ta;
    ld TT = T * T * T * T;

    //Nonlinear Energy Balance
    ld dTdt = (1.0/(m*Cp)) * (U*A*(Ta-T) + eps * sigma * A * (TA - TT) + alpha*Q);
    return dTdt;
}

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
  Serial.println(err);
  Serial.print("prev_err : ");
  Serial.println(prev_err);
  integral += err * dt;
  derivative = (err - prev_err) / dt;

  ld Pout = Kp * err;//proportional term
  ld Iout = Ki * integral;//integral term
  ld Dout = Kd * derivative;//derivative term
  ld output = Pout + Iout + Dout;//total output
  // Serial.print("P : ");
  // Serial.print(Pout);
  // Serial.print(" I : ");
  // Serial.print(Iout);
  Serial.print("set point : ");
  Serial.println(setpoint);
  // Serial.print("output : ");
  // Serial.println(output);
  //restrict to max/min
  if( output > hi ) output = hi;
  else if( output < lo ) output = lo;
  Serial.print("output : ");
  Serial.println(output);
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
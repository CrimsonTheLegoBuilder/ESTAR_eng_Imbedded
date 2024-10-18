#ifndef _SPRAY_H_
#define _SPRAY_H_

#include <Arduino.h>
#include <stdio.h>
#include "systemConfig.h"

struct Spray {
  int pump_pin;
  int air_pin;
  int water_pin;
  int pressure_pin1;
  // int pressure_pin2;
  int pump_state;
  int air_state;
  int water_state;
  int prs_hi, prs_lo;
  int cur_pressure;
  Spray(int hi = 700, int lo = 300, int p = PRESSURE_PIN, int a = AIR_PIN, int w = WATER_PIN, int prs = PRESSURE_PIN) : 
    pump_pin(p), air_pin(AIR_PIN), water_pin(w), pressure_pin1(prs) {
      prs_hi = hi;
      prs_lo = lo;
    }
  void init();
  void change_pressure_limit(int hi, int lo);
  void shut_off();
  void pump_on();
  void pump_off();
  void air_on();
  void air_off();
  void water_on();
  void water_off();
  int pressure_read();
};

#endif
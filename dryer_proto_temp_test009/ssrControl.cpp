#include "Arduino.h"
#ifndef _SSR_SOURCE_
#define _SSR_SOURCE_

int htpmp = 4, ptc = 5, fan = 6;
int interrupt_pin = 18;//2 - 0, 3 - 1, 18 - 5, 19 - 4, 20 - 3, 21 - 2

#include "ssrControl.h"

void ssr::init(int heatpump_, int ptc_, int fan_) {
  htpmp = heatpump_;
  ptc = ptc_;
  fan = fan_;
  pinMode(htpmp, OUTPUT);
  pinMode(ptc, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(interrupt_pin, INPUT_PULLUP);
}

void ssr::ptc_control(int w) {//PID control
  /*
  
  transfer fn'
  
  */
  int x = map(w, 0, 100, 0, 255);
  analogWrite(ptc, x);
}

void ssr::heatpump_control(int w) {//ON/OFF control
  /*
  
  transfer fn'
  
  */
  int x = (w > 50) * 5;
  digitalWrite(htpmp, x);
}

void ssr::fan_control(int w) {
  /*
  
  transfer fn'
  
  */
  int x = map(w, 0, 100, 0, 255);
  analogWrite(ptc, x);
}

void ssr::turn_on(int w) {
  ssr::heatpump_control(w);
  ssr::ptc_control(w);
  ssr::fan_control(w);
}
void ssr::turn_off() {
  ssr::heatpump_control(0);
  ssr::ptc_control(0);
  ssr::fan_control(0);
}

void ssr::run_dryer(bool BUTTON_STATE, bool changed, int w) {
  if (changed) {
    if (BUTTON_STATE) turn_on(w);
    else turn_off();
  }
}

bool ssr::detect_disturbance() {
  return digitalRead(interrupt_pin);
}


#endif
#include "Arduino.h"
#ifndef _LEDNODE_H_
#define _LEDNODE_H_

bool LED::init() {
  pinMode(detector, INPUT);
  return 1;
}

bool LED::write(const int& brt) {
  analogWrite(RED, brt);
  return 1;//if return 1, PID node work
}

bool LED::led_off(const int& brt) {
  analogWrite(RED, 0);
  return 0;//if return 0, PID node doesn't work
}

bool LED::slowly_down(int& brt) {
  if (brt <= 0) return 0;
  if (brt > 0) brt = max(0, brt - 20);
  analogWrite(RED, brt);
  return 0;//if return 0, PID node doesn't work
}

bool LED::run_led(const int& mode, int& brt) {
  // if (mode == NORMAL) return LED::write(brt);
  // else if (mode == DECREASE) return LED::slowly_down(brt); 
  // else if (mode == STOP) return LED::led_off(brt);

  switch (mode) {
    case NORMAL: return LED::write(brt);
    case DECREASE: return LED::slowly_down(brt);
    case STOP: return LED::led_off(brt);
    default:
    Serial.println("error occured");
    return LED::led_off(brt);
  }
}

bool LED::detect_disturbance() {
  if (digitalRead(detector) == HIGH) return 1;
  else return 0;
}

#endif
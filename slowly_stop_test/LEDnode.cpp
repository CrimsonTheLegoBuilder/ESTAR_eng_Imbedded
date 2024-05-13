#include "Arduino.h"
#ifndef _LEDNODE_H_
#define _LEDNODE_H_

bool LED::init() {
  pinMode(detector, INPUT);
  return 1;
}

bool LED::detect_disturbance() {
  if (digitalRead(detector) == LOW) return 1;
  else return 0;
  //이전 상태와 읽어낸 값이 다르다면 그 때 변화했음을 반환?

}

inline int LED::detect_disturbances(int& prev) {
  if (digitalRead(detector) != prev) {
    if (prev == HIGH) {//door open
      prev = LOW;
      return 1;
    }
    else {//door close
      prev = HIGH;
      return 0;
    }
  }
  return -1;//no change
}

int LED::write(const int& brt) {
  analogWrite(RED, brt);
  return 1;//if return 1, PID node work
}

int LED::led_off(const int& brt) {
  analogWrite(RED, 0);
  return 0;//if return 0, doesn't work
}

int LED::slowly_down(int& brt) {
  if (brt <= 0) return 0;
  if (brt > 0) brt = max(0, brt - 20);
  analogWrite(RED, brt);
  return -1;//if return -1, slowly power down
}

int LED::run_led(const int& mode, const int& door_state, int& brt) {
  // if (mode == NORMAL) return LED::write(brt);
  // else if (mode == DECREASE) return LED::slowly_down(brt); 
  // else if (mode == STOP) return LED::led_off(brt);
  if (!door_state) return -2;
  switch (mode) {
    case NORMAL: return LED::write(brt);
    case DECREASE: return LED::slowly_down(brt);
    case STOP: return LED::led_off(brt);
    default:
    Serial.println("error occured");
    return LED::led_off(brt);
  }
}

#endif
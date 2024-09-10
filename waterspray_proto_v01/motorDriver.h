#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "pinConfig.h"

struct Motor {
  int dir_pin;
  int spd_pin;
  int spd, dir;
  int teeth;
  volatile unsigned long last_pulse_time;
  volatile unsigned long pulse_interval;
  volatile bool direction_changed;

  
  void onPulse() {
    unsigned long current_pulse_time = micros();
    if (current_pulse_time - last_pulse_time >= debounce_delay) { // 디바운싱 처리
      pulse_interval = (current_pulse_time - last_pulse_time);
      last_pulse_time = current_pulse_time;
      //if (!dir) return;
      if (cnt >= teeth) {
        direction_changed = true;
      }
    }
  }
}

#endif
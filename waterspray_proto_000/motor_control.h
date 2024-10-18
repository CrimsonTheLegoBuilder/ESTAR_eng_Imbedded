// #include "esp32-hal-gpio.h"
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "systemConfig.h"

struct Motor {
  int dir_pin;
  int spd_pin;
  int btn_pin1;
  int btn_pin2;

  
#ifdef DEBUG 
  volatile int interrupt_debugger;
#endif



#ifdef DEBUG 
    interrupt_debugger = 0;
#endif
  }



  void DEBUG_();
};

#endif
#ifndef _BOARDCONFIG_H_
#define _BOARDCONFIG_H_

#include <Arduino.h>

/*
  programs for ESP32.
  do not uproad this ino.project on Arduino.
  Maybe this project fuck Arduino. 
*/

#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8
#define PWM_CHANNEL_1 0
#define PWM_CHANNEL_2 1

#define MOTOR_1_DIR 26
#define MOTOR_1_PWM 27
#define MOTOR_2_DIR 14
#define MOTOR_2_PWM 12

#define SWITCH_1 36
#define SWITCH_2 39
#define SWITCH_3 34
#define SWITCH_4 35

#define PHOTO 32
#define WATER_LEVEL 33
#define PRESSURE 25


#endif
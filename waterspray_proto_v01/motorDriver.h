#include "esp32-hal-gpio.h"
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "systemConfig.h"
#include "pidControl.h"

struct Motor {
  int dir_pin;
  int spd_pin;
  int btn_pin1;
  int btn_pin2;
  int sensor_pin;
  volatile float rpm;
  volatile float rad_;
  volatile int spd, dir;
  volatile int teeth;
  volatile int cnt;
  volatile unsigned long last_pulse_time;
  volatile unsigned long pulse_interval;
  volatile bool direction_changed;
  float target;
  PID pid;
    
  volatile bool interrupt_flag;
  volatile unsigned long last_interrupt_time;
  
#ifdef DEBUG 
  volatile int interrupt_debugger;
#endif

  Motor(int dirPin, int spdPin, int btn1, int btn2, int sensorPin = -1, int t = -1)
  : dir_pin(dirPin), spd_pin(spdPin), btn_pin1(btn1), btn_pin2(btn2) {
    pinMode(dir_pin, OUTPUT);//digital
    // pinMode(spd_pin, OUTPUT);//analog
    ledcAttach(spd_pin, pwm_freq, pwm_res);
    pinMode(btn_pin1, INPUT_PULLDOWN);//digital
    pinMode(btn_pin2, INPUT_PULLDOWN);//digital
    sensor_pin = sensorPin;
    if (~sensorPin) pinMode(sensor_pin, INPUT_PULLUP);
    rpm = 0;
    rad_ = 0;
    spd = 0;
    dir = 0;
    teeth = t;
    cnt = 0;
    last_pulse_time = 0;
    pulse_interval = 0;
    direction_changed = 0;
    target = 0;
    pid.init(0, 0, 1e-4, 60, 255);

    interrupt_flag = 0;
    last_interrupt_time = 0;

#ifdef DEBUG 
    interrupt_debugger = 0;
#endif
  }

  void begin();
  void set_speed(int s = 0);
  int get_pow(float speed = 0);
  float get_speed(int pow = 0);
  void set_speed(float rpm_ = 0);
  void set_direction(int d = 1);
  void toggle();
  void set_point(float point);
  float cal_speed(float cur);
  void set_speed_limit(int speed, float the, float ratio = .3);
  float rad();
  float degree();
  void count_();
  void stop();
  static void photoISR1();
  static void btnISR1();
  static void btnISR2();
  static void sensorISR2();

  void handle_pulse();
  static Motor* motor1;
  static Motor* motor2;

  void DEBUG_();
};

// //object motor 1, 2
extern Motor motor1_;
extern Motor motor2_;

#endif
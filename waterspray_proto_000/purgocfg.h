#ifndef _PURGOCFG_H_
#define _PURGOCFG_H_

#include "Arduino.h"

#define DEBUG

#define TOL 1e-7

//pin map
#define MOTOR1_DIR 26
#define MOTOR1_SPD 27
#define BTN1_PIN 36
#define BTN2_PIN 39
#define PHOTO_PIN 32

#define MOTOR2_DIR 14
#define MOTOR2_SPD 13
#define BTN3_PIN 34
#define BTN4_PIN 35

#define LEVEL_PIN 33
#define PRESSURE_PIN 25

#define RX_PIN 18
#define TX_PIN 17

//dwin address
#define WRITE_DATA 0x82
#define READ_DATA 0x83

//macro
#define FORWARD 1
#define REVERSE -1

#define CW 0
#define CCW 1

typedef long long ll;
typedef double ld;

enum EventType {
  RUN_DRYER=0X3A,
  STOP_DRYER,
  SET_TEMPERATURE,
  CHANGE_TEMPERATURE,
  EMERGENCY_STOP,
  PAUSE_DRYER,
  TEMP_ADD,//0X40
  HUMI_ADD,
};

typedef enum {
  STATE_IDLE,
  STATE_INIT,
  STATE_HOMEBUMP,
  STATE_ROTATE,
  STATE_COMPLETE,
} TurntableState_t;

typedef enum {
  EVENT_HOMEBUMP_FIRST,
  EVENT_ROTATE_CW,
  EVENT_HOMEBUMP_SECOND,
  EVENT_ROTATE_CCW,
} ButtonEvent_t;

const unsigned long debounce_delay = 200;
const unsigned long btn_debounce_delay = 1;

const unsigned long pwm_freq = 20000;
const unsigned long pwm_res = 8;

// const unsigned long debounce_delay = 5; // 5ms debounce

ld norm(ld t);
float norm(float t);

#endif
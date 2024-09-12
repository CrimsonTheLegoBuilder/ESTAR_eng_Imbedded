#ifndef _PINCONFIG_H_
#define _PINCONFIG_H_

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

#define WRITE_DATA 0x82
#define READ_DATA 0x83

#define FORWARD 1;
#define REVERSE -1;

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

const unsigned long debounce_delay = 200;

#endif
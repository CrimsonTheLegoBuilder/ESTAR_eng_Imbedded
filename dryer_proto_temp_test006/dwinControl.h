#ifndef _DWIN_CONTROL_H_
#define _DWIN_CONTROL_H_

#include "dwinControl.h"
#include <SoftwareSerial.h>
// #include 

typedef long long ll;
typedef double ld;

const byte rx = 12;
const byte tx = 13;
const int LEN = 9;

SoftwareSerial mySerial(rx, tx);

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
#define temperature_add 0x61
#define humidity_add 0x62
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, 0x82, temperature_add, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x01 };

struct Data {
  float humi, temp;
  Data(float h = 0, float t = 0) : humi(h), temp(t) {}
};

namespace dwin {
  float read_setpoint();
  void print_humitemp(float temp, float humi);
}

#endif
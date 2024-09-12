#ifndef _DWIN_CONTROL_H_
#define _DWIN_CONTROL_H_

#include <stdint.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "systemConfig.h"

#define temperature_add 0x61
#define humidity_add 0x62

typedef long long ll;
typedef double ld;

namespace dwin {
  // class uart {
  // public:
  //   uart(HardwareSerial& port);
  //   void begin(long baudrate);
  // private:
  //   HardwareSerial& serial;
  // };
  void setup();
  // bool read(Event& info);
  // void write(Event& event);
  //bool read_setpoint(int& temp);
  // void print_humitemp(float temp, float humi);
}

#endif
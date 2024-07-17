#include <stdint.h>
#ifndef _DWIN_CONTROL_H_
#define _DWIN_CONTROL_H_

#include <Arduino.h>
#include <HardwareSerial.h>

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
  struct Info {
    unsigned char vp_address;
    int flag;
  };
  void setup();
  Info read(int& temp);
  bool read_setpoint(int& temp);
  void print_humitemp(float temp, float humi);
}

#endif
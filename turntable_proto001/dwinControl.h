#ifndef _DWIN_CONTROL_H_
#define _DWIN_CONTROL_H_

#include <Arduino.h> 

#define temperature_add 0x61
#define humidity_add 0x62

typedef long long ll;
typedef double ld;

namespace dwin {
  struct Info {
    unsigned char vp_address;
    int flag;
  };
  void setup();
  Info read(int& temp);
}

#endif
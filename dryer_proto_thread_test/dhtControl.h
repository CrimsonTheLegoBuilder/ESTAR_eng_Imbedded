#ifndef _DHT_CONTROL_H_
#define _DHT_CONTROL_H_

#include <Arduino.h>

#define DHT_PIN 3
#define DHT_TYPE DHT11

struct Data {
  float humi, temp;
  Data(float h = 0, float t = 0) : humi(h), temp(t) {}
};

namespace dht {
  void init();
  Data check(bool f = 0);
};

#endif
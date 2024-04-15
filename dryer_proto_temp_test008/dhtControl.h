#ifndef _DHT_CONTROL_H_
#define _DHT_CONTROL_H_

#include <Arduino.h>

#define DHT_PIN 3
#define DHT_SENSOR_TYPE DHT_TYPE_11

struct Data {
  float humi, temp;
  Data(float h = 0, float t = 0) : humi(h), temp(t) {}
};

namespace dht {
  bool check(float& t, float& h);
  void init(float& t, float& h);
};

#endif
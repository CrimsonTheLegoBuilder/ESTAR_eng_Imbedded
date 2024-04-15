#ifndef _DHT_CONTROL_SOURCE_
#define _DHT_CONTROL_SOURCE_

#include "dhtControl.h"
#include "DHT_Async.h"

DHT_Async dht11(DHT_PIN, DHT_TYPE_11);

bool dht::check(float& t, float& h) {
  bool f = dht11.measure(&t, &h);
  // Serial.println(t);
  // Serial.println(h);
  return f;
}

void dht::init(float& t, float& h) {
  int cnt = 100;
  while (cnt--) {
    if (dht::check(t, h)) break;
    Serial.println("wait...");
  }
  Serial.println("DHT11 initialized.");
  return;
}

#endif
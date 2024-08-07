#ifndef _DHT_CONTROL_SOURCE_
#define _DHT_CONTROL_SOURCE_

#include "dhtControl.h"
#include "DHT_Async.h"

DHT_Async dht11(DHT_PIN, DHT_TYPE_11);

// void dht::init() {
//   Serial.println("DHT11 initialized.");
//   return;
// }

bool dht::check(float& t, float& h) {
  bool f = dht11.measure(&t, &h);
  Serial.println(t);
  Serial.println(h);
  return f;
}

void dht::init(float& t, float& h) {
  while (!dht::check(t, h)) Serial.println("wait...");
  Serial.println("DHT11 initialized.");
  return;
}

// float h = 0, t = 0, tf = 0;

#endif
#ifndef _DHT_CONTROL_SOURCE_
#define _DHT_CONTROL_SOURCE_

#include "dhtControl.h"
#include "DHT_Async.h"

DHT_Async dht11(DHT_PIN, DHT_TYPE_11);

void dht::init() {
  // dht11.begin();
  Serial.println("DHT11 initialized.");
  return;
}

float h = 0, t = 0, tf = 0;
Data dht::check(bool f) {
  dht11.measure(&t, &h);
  Serial.println(t);
  Serial.println(h);
  if (f) return Data(h, tf);
  return Data(h, t);
}

#endif
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

Data dht::check(bool f) {
  float h = 0, t = 0, tf = 0;
  // h = dht11.readHumidity();
  // t = dht11.readTemperature();
  dht11.measure(&t, &h);
  // tf = dht11.computeHeatIndex(t, h, false);
  // Serial.print(h);
  // Serial.print(" ");
  // Serial.print(t);
  // Serial.print(" ");
  // Serial.println(tf);
  if (f) return Data(h, tf);
  return Data(h, t);
}

#endif
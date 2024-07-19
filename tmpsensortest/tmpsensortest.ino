#include "Arduino.h"

const int t_pin = 3;
const int h_pin = 4;
const int VDD = 3300;

void setup() {
  // put your setup code here, to run once:
  pinMode(t_pin, INPUT);
  pinMode(h_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int vt, vrh;
  vt = analogRead(0);
  Serial.println(vt);
  vrh = analogRead(1);
  Serial.println(vrh);
  vt = map(vt, 0, 700, 0, 3300);
  vrh = map(vrh, 0, 700, 0, 3300);
  float t = -66.875 + 218.75 * vt / VDD;
  float h = -(19.7 / 0.54) + (100 / 0.54 * vrh / VDD);
  Serial.print("temp:: ");
  Serial.println(t);
  Serial.println(vt);
  Serial.println(vt * 5. / 3.3);
  Serial.print("humi:: ");
  Serial.println(h);
  Serial.println(vrh);
  Serial.println(vrh * 5 / 3.3);
  delay(2000);
}

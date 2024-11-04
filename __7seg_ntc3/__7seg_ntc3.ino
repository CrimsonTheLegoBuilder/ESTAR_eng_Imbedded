#include "__estar_config.h"
#include "_7seg.h"
#include "_thermo.h"

TwoSeg S;
Thermo T(B_pin, R_pin, W_pin);

void setup() {
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:

  float t = T.read_temper(1);
  Serial.println(t);
  S.print(t);
  delay(500);
}

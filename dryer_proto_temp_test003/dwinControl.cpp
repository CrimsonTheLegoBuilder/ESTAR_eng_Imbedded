#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "dwinControl.h"
// #include 

float dwin::read_setpoint() {
  int temp = -1;
  if (mySerial.available()) {
    for (int i = 0; i <= LEN; i++) Buffer[i] = mySerial.read();
    // for (int i = 0; i <= LEN; i++) {
    //   Serial.print(Buffer[i], HEX);
    //   Serial.print(" ");
    // }
    // Serial.println();
    // if (Buffer[0] == 0X5A && Buffer[4] == 0X56) {
    //   Serial.println(Buffer[8], HEX);
    //   if (Buffer[8] == 1) digitalWrite(light, HIGH);
    //   else digitalWrite(light, LOW);
    // }
    if (Buffer[0] == 0X5A && Buffer[4] == 0X55) {
      temp = Buffer[8];
      Serial.println(temp);
    }
  }
  return temp;
}
void dwin::print_humitemp(float temp, float humi) {
  Temperature[6] = highByte(temp);
  Temperature[7] = lowByte(temp);
  mySerial.write(Temperature, 8);

  Humidity[6] = highByte(humi);
  Humidity[7] = lowByte(humi);
  mySerial.write(Humidity, 8);

  return;
}

#endif
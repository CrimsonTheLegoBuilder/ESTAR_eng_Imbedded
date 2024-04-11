#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "dwinControl.h"
#include <SoftwareSerial.h>
// #include 

const byte rx = 12;
const byte tx = 13;
const int LEN = 12;

SoftwareSerial mySerial(rx, tx);

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, 0x82, temperature_add, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x01 };

void dwin::setup() {
  mySerial.begin(115200);
}

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
      // Serial.println(temp);
    }
  }
  return temp;
}

void dwin::print_humitemp(float temp, float humi) {
  Temperature[6] = highByte((int)temp);
  Temperature[7] = lowByte((int)temp);
  mySerial.write(Temperature, 8);

  Humidity[6] = highByte((int)humi);
  Humidity[7] = lowByte((int)humi);
  mySerial.write(Humidity, 8);

  return;
}

#endif
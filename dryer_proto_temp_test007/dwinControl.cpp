#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "dwinControl.h"
#include <SoftwareSerial.h>
// #include 

const byte rx = 12;
const byte tx = 13;
const int LEN = 50;

SoftwareSerial mySerial(rx, tx);

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, 0x82, temperature_add, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x01 };

void dwin::setup() {
  mySerial.begin(115200);
}

extern int __bss_end;
extern void *__brkval;

int memoryPrintl() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}

float dwin::read_setpoint() {
  int temp = -1;
  if (mySerial.available()) {
    // for (int i = 0; i <= LEN; i++) Buffer[i] = mySerial.read();
    int i = 0;
    while (mySerial.available() > 0) Buffer[i++] = mySerial.read();
    if (Buffer[0] == 0X5A && Buffer[4] == 0X55) {
      temp = Buffer[8];
      Serial.println(memoryPrintl());
    }
    return temp;
  }
  else {
      while (mySerial.available() > 0) mySerial.read();
      return -1;
  }
  //return temp;
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
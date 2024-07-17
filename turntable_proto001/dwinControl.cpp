#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "dwinControl.h"
#include <HardwareSerial.h>

// ESP32
const int RXD1 = 16;
const int TXD1 = 17;
const int LEN = 64;

HardwareSerial mySerial(1); // UART1

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };

void dwin::setup() {
  mySerial.begin(38400);
}

extern int __bss_end;
extern void *__brkval;

int memoryPrintl() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}

dwin::Info dwin::read(int& temp) {
  int n;
  if (n = mySerial.available()) {
    int i = 0;
    // for (i = 0; i < 9; i++) Buffer[i] = mySerial.read();
    while (mySerial.available() > 0) Buffer[i++] = mySerial.read();
    Serial.println(i);
    if (Buffer[0] == 0X5A) {
      unsigned char addr = Buffer[4];
      switch (addr) {
        case 0X55:
          if (i == 9) {
            temp = Buffer[8];
            Serial.print(temp);
            Serial.println("valid check ok");
            return { 0X55, 1 };
          }
          return { 0X55, 0 };
        case 0X60:
          if (i == 9) {
            Serial.println("run main");
            return { 0X60, Buffer[8] };
          }
          return { 0X60, -1 };
        default:
          return { 0X60, -2 };
      }
    }
  }
  else {
    while (mySerial.available() > 0) mySerial.read();
    return { 0X60, -2 };
  }
}

#endif
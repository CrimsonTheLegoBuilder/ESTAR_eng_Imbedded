#include "Arduino.h"
#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "dwinControl.h"
#include <SoftwareSerial.h>
// #include 

const byte rx = 11;
const byte tx = 10;
const int LEN = 64;

SoftwareSerial mySerial(rx, tx);

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, 0x82, temperature_add, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x01 };

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

bool dwin::read_setpoint(int& temp) {
  int n;
  if (n = mySerial.available()) {
    int i = 0;
    for (i = 0; i < 9; i++) Buffer[i] = mySerial.read();
    //while (mySerial.available() > 0) Buffer[i++] = mySerial.read();
    Serial.println(i);
    for (int j = 0; j <= 9; j++) Serial.println(Buffer[j]);
    if (Buffer[0] == 0X5A && Buffer[4] == 0X55 && i == 9) {
      temp = Buffer[8];
      Serial.print(temp);
      Serial.println("valid check ok");
    }
    else Serial.println("valid check fuck");
    //while (mySerial.available() > 0) mySerial.read();
    return 1;
  }
  else {
    while (mySerial.available() > 0) mySerial.read();
    return 0;
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
  while (mySerial.available() > 0) mySerial.read();
  return;
}

#endif
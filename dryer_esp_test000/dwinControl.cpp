#include "HardwareSerial.h"
#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "dwinControl.h"

// ESP32

const int LEN = 64;

HardwareSerial uart(1); // UART1

// dwin::uart::uart(HardwareSerial& port) : serial(port) {}

// void dwin::uart::begin(long baudrate, uint8_t rx, uint8_t tx) {
//   serial.begin(baudrate, SERIAL_8N1, rx, tx);
// }

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, 0x82, temperature_add, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x01 };

void dwin::setup() {
  uart.begin(38400);
}

extern int __bss_end;
extern void *__brkval;

int memoryPrintl() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}

/*
info에 필요한 자료
LCD로부터 읽어들인 데이터의 목표와 내용


*/
dwin::Info dwin::read(int& temp) {
  int n;
  if (n = uart.available()) {
    int i = 0;
    // for (i = 0; i < 9; i++) Buffer[i] = mySerial.read();
    while (uart.available() > 0) Buffer[i++] = uart.read();
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
    while (uart.available() > 0) uart.read();
    return { 0X60, -2 };
  }
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
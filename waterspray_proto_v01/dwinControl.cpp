#ifndef _DWIN_CONTROL_SOURCE_
#define _DWIN_CONTROL_SOURCE_

#include "HardwareSerial.h"
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
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, WRITE_DATA, TEMP_ADD, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, WRITE_DATA, HUMI_ADD, 0x00, 0x00, 0x01 };
unsigned char arr[8] = { 0x5a, 0xa5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x01 };

// extern int __bss_end;
// extern void *__brkval;

// int memoryPrintl() {
//   int freeMemory;
//   if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
//   else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
//   return freeMemory;
// }

/*
LCD로부터 읽어들인 데이터의 목표와 내용만 간단하게 실어서 내보내도록 단순화.
각 함수의 기능을 세분화해서 정확히 분담 받은 작업만 수행하게 만들어야 함.

*/

void dwin::setup() {
  // uart.begin(38400);
  uart.begin(38400, SERIAL_8N1, 19, 18);
}

// bool dwin::read(Event& info) {
//   int n;
//   if (uart.available()) {
//     int i = 0;
//     // for (i = 0; i < n; i++) Buffer[i] = mySerial.read();
//     while (uart.available() > 0) Buffer[i++] = uart.read();
//     Serial.print("i:: ");
//     Serial.println(Buffer[4]);
//     Serial.println(Buffer[8]);
    
//     if (Buffer[0] == 0X5A) {//valid check
//       //valid check process
//       unsigned char addr = Buffer[4];
//       int val = Buffer[8];
//       info.type = addr;
//       info.val = val;
//       return 1;
//     }
//     else {
//       info = { 0X60, -2 };//unval
//       return 1;
//     }
//   }
//   else {
//     while (uart.available() > 0) uart.read();
//     info = { 0X60, -2 };//unval
//     return 0;
//   }
// }

// void dwin::write(Event& event) {
//   arr[4] = event.type;
//   arr[6] = highByte((int)event.val);
//   arr[7] = lowByte((int)event.val);
//   uart.write(arr, 8);
//   //while (uart.available() > 0) uart.read();
//   return;
// }

// bool dwin::read_setpoint(int& temp) {
//   int n;
//   if (n = mySerial.available()) {
//     int i = 0;
//     for (i = 0; i < 9; i++) Buffer[i] = mySerial.read();
//     //while (mySerial.available() > 0) Buffer[i++] = mySerial.read();
//     Serial.println(i);
//     for (int j = 0; j <= 9; j++) Serial.println(Buffer[j]);
//     if (Buffer[0] == 0X5A && Buffer[4] == 0X55 && i == 9) {
//       temp = Buffer[8];
//       Serial.print(temp);
//       Serial.println("valid check ok");
//     }
//     else Serial.println("valid check fuck");
//     //while (mySerial.available() > 0) mySerial.read();
//     return 1;
//   }
//   else {
//     while (mySerial.available() > 0) mySerial.read();
//     return 0;
//   }
//   //return temp;
// }

// void dwin::print_humitemp(float temp, float humi) {
//   Temperature[6] = highByte((int)temp);
//   Temperature[7] = lowByte((int)temp);
//   uart.write(Temperature, 8);

//   Humidity[6] = highByte((int)humi);
//   Humidity[7] = lowByte((int)humi);
//   uart.write(Humidity, 8);
//   while (uart.available() > 0) uart.read();
//   return;
// }


#endif
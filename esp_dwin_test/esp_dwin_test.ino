// #include "HardwareSerial.h"
const int LEN = 64;

// HardwareSerial uart(1); // UART1
#define RX1 19
#define TX1 18
#define PUMP_PERIOD 10000
#define WRITE_DATA 0x82
#define READ_DATA 0x83

enum EventType {
  RUN_DRYER=0X3A,
  STOP_DRYER,
  SET_TEMPERATURE,
  CHANGE_TEMPERATURE,
  EMERGENCY_STOP,
  PAUSE_DRYER,
  TEMP_ADD,//0X40
  HUMI_ADD,
};

unsigned char Buffer[LEN];
byte set_config[7] = { 0x5a, 0xa5, 0x04, 0x83, 0x00, 0x0f, 0x01 };
unsigned char Temperature[8] = { 0x5a, 0xa5, 0x05, WRITE_DATA, TEMP_ADD, 0x00, 0x00, 0x01 };
unsigned char Humidity[8] = { 0x5a, 0xa5, 0x05, WRITE_DATA, HUMI_ADD, 0x00, 0x00, 0x01 };
unsigned char arr[8] = { 0x5a, 0xa5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x01 };

struct Event {
  unsigned char type;
  int val;
};

Event info;

void setup() {
  // put your setup code here, to run once:
  // uart.begin(38400, SERIAL_8N1, 19, 18);
  Serial1.begin(38400);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("read:: ");
  Serial.println(read(info));
  Serial.print("info.type:: ");
  Serial.println(info.type, HEX);
  Serial.print("info.val:: ");
  Serial.println(info.val);
  delay(1000);
}

bool read(Event& info) {
  int n;
  if (Serial1.available()) {
    int i = 0;
    // for (i = 0; i < n; i++) Buffer[i] = mySerial.read();
    while (Serial1.available() > 0) Buffer[i++] = Serial1.read();
    Serial.print("i:: ");
    Serial.println(i);
    for (int j = 0; j < i; j++) {
          Serial.print(Buffer[j], HEX);
          Serial.print(" ");
    }
    Serial.println(" ");
    Serial.println(Buffer[4], HEX);
    Serial.println(Buffer[8], HEX);
    
    if (Buffer[0] == 0X5A) {//valid check
      //valid check process
      unsigned char addr = Buffer[4];
      int val = Buffer[8];
      info.type = addr;
      info.val = val;
      return 1;
    }
    else {
      info = { 0X60, -1 };//unval
      return 1;
    }
  }
  else {
    while (Serial1.available() > 0) Serial1.read();
    info = { 0X60, -2 };//unval
    return 0;
  }
}

// bool read(Event& info) {
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
//       info = { 0X60, -1 };//unval
//       return 1;
//     }
//   }
//   else {
//     while (uart.available() > 0) uart.read();
//     info = { 0X60, -2 };//unval
//     return 0;
//   }
// }
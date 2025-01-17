// HardwareSerial &CM1106 = Serial1;
#include <SoftwareSerial.h>

SoftwareSerial CM1106(13, 11);
unsigned char data[4] = { 0x11, 0x01, 0x01, 0xED };
unsigned char buff[8];

unsigned char recv_cnt;
unsigned int ppm;

void send_cmd() {
  unsigned int i;
  for (i = 0; i < 4; i++) {
    CM1106.write(data[i]);
    delay(1);
  }
}

unsigned char crc() {
  unsigned char cnt, sum_ = 0;
  for (cnt = 0; cnt < 7; cnt++) sum_ += buff[cnt];
  return 256 - sum_;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT); 
  pinMode(13, INPUT); 
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  CM1106.begin(9600);
  while (!CM1106) { Serial.println("fuck"); }
}

void loop() {
  // put your main code here, to run repeatedly:
  //CO2
  send_cmd();
  while (1) {
    if (CM1106.available()) {
      // Serial.print("z");
      buff[recv_cnt++] = CM1106.read();
      if (recv_cnt == 8) { recv_cnt = 0; break; }
    }
  }
  if (crc()) {
    ppm = buff[3] << 8 | buff[4];
    Serial.print("PPM: ");
    Serial.println(ppm);
  }
  else Serial.println("What the FUCK???");
}

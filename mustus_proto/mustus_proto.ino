#include <SoftwareSerial.h>
#include "mustus_config.h"
#include "DHT.h"

/*

일단 폴링 방식으로 구현한 무스투스 프로토타입 코드
사용되는 센서는 총 7개이며, 이 중 아날로그 형태로 데이터를 뽑아낼 수 있는 센서들을 분류 요망
아날로그 데이터는 데이터 로거로 넘겨서 분석을 위해 사용할 수 있도록 디자인해야 함.
아두이노는 완벽한 아날로그 신호를 낼 수 없으므로
아날로그 신호를 내는 장비는 그대로 입력을 할 수 있도록 하고,
가공이 필요한 데이터가 문제인데...

*/

#include <Wire.h>
#include <MS5611.h>

DHT dht(TEMP, DHT22);
// SoftwareSerial CM1106(10, 11);
HardwareSerial &CM1106 = Serial1;
// SoftwareSerial ZE15(12, 13); // RX, TX
HardwareSerial &ZE15 = Serial2; // RX, TX
HardwareSerial &pms = Serial3; // RX, TX
MS5611 ms5611;

static const int REQUEST_CNT = 9;
static const int RESPONSE_CNT = 9;
unsigned char data[4] = { 0x11, 0x01, 0x01, 0xED };
uint8_t getCOppm[REQUEST_CNT] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00,0x79};
unsigned char buff[8];
unsigned char recv_cnt;
unsigned int ppm;
double referencePressure;


float measurement;

#define WAIT_READ_TIMES 100
#define WAIT_READ_DELAY 10

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

void writeCommand(uint8_t cmd[], uint8_t *response) {
  ZE15.write(cmd, REQUEST_CNT);
  ZE15.flush();

  if (response != NULL) {
    int i = 0;
    while (ZE15.available() <= 0) {
      if (++i > WAIT_READ_TIMES) {
        Serial.println("can't get ZE15CO response.");
        return;
      }
      delay(WAIT_READ_DELAY);
    }
    ZE15.readBytes(response, RESPONSE_CNT);
  }
}

void checkSettings() {
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(10, INPUT);
  pinMode(11, OUTPUT); 
  pinMode(5, INPUT);
  pinMode(6, INPUT); 
  CM1106.begin(9600);
  while (!CM1106);
  ZE15.begin(9600);
  while (!ms5611.begin()) {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }
  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();
  // Check settings
  checkSettings();
  pms.begin(9600);
  while (!pms);
}

void loop() {
  //VOCs
  int vocs = analogRead(VOCS);
  Serial.print("vocs: ");
  Serial.println(vocs);

  //t, h
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h); //습도가 출력 됩니다.
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t); //온도가 출력됩니다.
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic); //열지수(체감온도)가 출력됩니다.
  Serial.println(" *C ");

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

  //TVOC
  // int hi = analogRead(2);
  // int lo = analogRead(3);
  int hi = digitalRead(5);
  int lo = digitalRead(6);
  Serial.print(hi);
  Serial.print(" ");
  Serial.println(lo);

  //CO
  float CO = 5. * (analogRead(4) / 1024.);
  Serial.print("CO: ");
  Serial.print(CO);
  Serial.println(" ");

  byte buf[RESPONSE_CNT-1];
  byte cheksum;
  writeCommand(getCOppm, buf);
  for (int i = 0; i < RESPONSE_CNT; i++) buf[i] = 0x0;
  writeCommand(getCOppm, buf);
  cheksum = (buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]);
  cheksum = (~cheksum)+1;
  if (buf[0] == 0xff && buf[1] == 0x86 && buf[8] == cheksum) measurement = (buf[2] * 256 + buf[3]) * 0.1;
  else measurement = -1;
  Serial.println(measurement);

  //baro
  // Read raw values
  uint32_t rawTemp = ms5611.readRawTemperature();
  uint32_t rawPressure = ms5611.readRawPressure();
  // Read true temperature & Pressure
  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();
  // Calculate altitude
  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
  Serial.println("--");
  Serial.print(" rawTemp = ");
  Serial.print(rawTemp);
  Serial.print(", realTemp = ");
  Serial.print(realTemperature);
  Serial.println(" *C");
  Serial.print(" rawPressure = ");
  Serial.print(rawPressure);
  Serial.print(", realPressure = ");
  Serial.print(realPressure);
  Serial.println(" Pa");
  Serial.print(" absoluteAltitude = ");
  Serial.print(absoluteAltitude);
  Serial.print(" m, relativeAltitude = ");
  Serial.print(relativeAltitude);
  Serial.println(" m");

  Serial.print("pms.avail: ");
  Serial.println(pms.available());
  if (pms.available() >= 32) {//신호 받은 갯수가 32개 이상일때
    int chksum=0;//체크섬 변수선언
    int pm1, pm25, pm10;
    unsigned char pms_data[32];//데이터 배열변수 선언
    for(int j = 0; j < 32; j++){//데이터 32개를 받는 반복
      pms_data[j] = pms.read();//데이터를 받음
      // switch (j) {//데이터 종류를 보기위한 구문
      //   case 0: Serial.print("시작1:"); break;
      //   case 1: Serial.print("시작2:"); break;
      //   case 2: Serial.print("길이1:"); break;
      //   case 3: Serial.print("길이2:"); break;
      //   case 4: Serial.print("데이터1-1:"); break;
      //   case 5: Serial.print("데이터1-2:"); break;
      //   case 6: Serial.print("데이터2-1:"); break;
      //   case 7: Serial.print("데이터2-2:"); break;
      //   case 8: Serial.print("데이터3-1:"); break;
      //   case 9: Serial.print("데이터3-2:"); break;
      //   case 10: Serial.print("데이터4-1:"); break;
      //   case 11: Serial.print("데이터4-2:"); break;
      //   case 12: Serial.print("데이터5-1:"); break;
      //   case 13: Serial.print("데이터5-2:"); break;
      //   case 14: Serial.print("데이터6-1:"); break;
      //   case 15: Serial.print("데이터6-2:"); break;
      //   case 16: Serial.print("데이터7-1:"); break;
      //   case 17: Serial.println(" "); Serial.print("데이터7-2:"); break;
      //   case 18: Serial.print("데이터8-1:"); break;
      //   case 19: Serial.print("데이터8-2:"); break;
      //   case 20: Serial.print("데이터9-1:"); break;
      //   case 21: Serial.print("데이터9-2:"); break;
      //   case 22: Serial.print("데이터10-1:"); break;
      //   case 23: Serial.print("데이터10-2:"); break;
      //   case 24: Serial.print("데이터11-1:"); break;
      //   case 25: Serial.print("데이터11-2:"); break;
      //   case 26: Serial.print("데이터12-1:"); break;
      //   case 27: Serial.print("데이터12-2:"); break;
      //   case 28: Serial.print("데이터13-1:"); break;
      //   case 29: Serial.print("데이터13-2:"); break;
      //   case 30: Serial.print("체크1:"); break;
      //   case 31: Serial.print("체크2:"); break;
      //   default: break;
      // }
      if (j == 31) {//데이터의 끝에서 줄바꿈
        Serial.println(pms_data[j]);
      }
      else {//아닐 경우 데이터 출력
        Serial.print(pms_data[j]);
        Serial.print(" ");
      }
      if (j<30)//데이터를 체크섬 변수에 계속 더함
        chksum+=pms_data[j];
    }

    while (pms.available()) pms.read();

    if (pms_data[30] != (unsigned char)(chksum>>8) || pms_data[31]!= (unsigned char)(chksum) ){
      return 0;
      Serial.println("데이터 체크 오류");//체크섬 데이터로 확인했을때 오류가 있을 때 
    }
    if (pms_data[0]!=0x42 || pms_data[1]!=0x4d ){
      return 0;
      Serial.println("시작 신호 오류");//시작신호에서부터 오류가 있을때
    }

    pm1 = pms_data[10]<<8 | pms_data[11];//수정된 출력 방법
    pm25 = pms_data[12]<<8 | pms_data[13];
    pm10 = pms_data[14]<<8 | pms_data[15];
    Serial.println("");
    Serial.print("1.0ug/m^3:");
    Serial.print(pm1);
    Serial.print("  2.5ug/m^3:");
    Serial.print(pm25);
    Serial.print("  10ug/m^3:");
    Serial.println(pm10);
    Serial.println("");
  } 


  delay(2000);
}

#ifndef _DATAINFO_H_
#define _DATAINFO_H_

#define WRITE_DATA 0x82
#define READ_DATA 0x83

//ESP32 pin map==========//

//dwin LCD 와의 UART 통신용 핀. dwin LCD의 UART2 와 엇갈려서 연결.
#define RX1 19
#define TX1 18

//LED 스트립 제어 신호용 DIO 핀. 3.3V로도 제어 가능.
#define RGB_PIN 5

//히트펌프, 팬, 원적외선 히터 제어용 핀. 추후 상세 배정 필요.
#define SSR_1 32
#define SSR_2 33
#define SSR_3 25

//도어 액추에이터, 이오나이저 OUT
#define RLY_1 26
#define RLY_2 27

//온습도 센서 1, 2 - max 3.3V 비례식으로 전압으로부터 수치를 계산.
#define TMP_1 36
#define HUM_1 39
#define TMP_2 34
#define HUM_2 35

//ESP32 pin map==========//

//hardware variable======//

#define PUMP_PERIOD 10000

/*
0X1A ~ 0X30
0X3A ~ 0X79
*/
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

struct Data {
  float humi, temp;
  Data(float h = 0, float t = 0) : humi(h), temp(t) {}
};

struct Event {
  unsigned char type;
  int val;
};

struct HeaterOrder {
  unsigned long pump_on_duration;
  unsigned long pump_off_duration;
  unsigned long ir_heater_pow;
  HeaterOrder(unsigned long O = 0, unsigned long F = PUMP_PERIOD, unsigned long P = 0) : 
    pump_on_duration(O),
    pump_off_duration(F),
    ir_heater_pow(P) {}
};

inline double sqr(const double& x);
inline double cube(const double& x);

#endif
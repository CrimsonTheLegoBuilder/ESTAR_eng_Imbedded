#ifndef _DATAINFO_H_
#define _DATAINFO_H_

#define WRITE_DATA 0x82
#define READ_DATA 0x83

enum EventType {
  /*
  0X1A ~ 0X30
  0X3A ~ 0X79
  */
  RUN_DRYER=0X3A,
  STOP_DRYER,
  SET_TEMPERATURE,
  CHANGE_TEMPERATURE,
  EMERGENCY_STOP,
  PAUSE_DRYER,
};
struct Data {
  float humi, temp;
  Data(float h = 0, float t = 0) : humi(h), temp(t) {}
};
struct Event {
  unsigned char type;
  int val;
};
inline double sqr(const double& x);
inline double cube(const double& x);

#endif
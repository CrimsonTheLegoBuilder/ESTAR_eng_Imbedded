#ifndef _MATHX_H_
#define _MATHX_H_

struct Data {
  float humi, temp;
  Data(float h = 0, float t = 0) : humi(h), temp(t) {}
};
inline double sqr(const double& x);
inline double cube(const double& x);

#endif
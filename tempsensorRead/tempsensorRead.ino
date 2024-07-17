#include <math.h>

//#define sqr(x) (x * x)
//#define cube(x) (x * x * x)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int v = analogRead(4);
  Serial.println(v);
  Serial.println(interpol(v));
  delay(500);
}

inline double cube(const double& x) {
    return x * x * x;
}

inline double sqr(const double& x) {
    return x * x;
}

inline double interpol(const double& resis) {
    // return -0.0445 * resis + 102.67;
    double ret = -6 * 1e-9 * cube(resis) + 4 * 1e-5 * sqr(resis) - 0.1112 * resis + 133.77;
    return ret;
    // R^2 = 0.998
}

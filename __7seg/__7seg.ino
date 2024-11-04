#include "__estar_config.h"
#include "_7seg.h"

TwoSeg T;

void setup() {}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i <= 99; i++) {
    T.print(i);
    delay(500);
  }
}

// SevenSeg A(
//   A_0, 
//   A_1, 
//   A_2, 
//   A_3, 
//   A_4, 
//   A_5, 
//   A_6, 
//   A_7
// );
// SevenSeg B(
//   B_0, 
//   B_1, 
//   B_2, 
//   B_3, 
//   B_4, 
//   B_5, 
//   B_6, 
//   B_7
// );
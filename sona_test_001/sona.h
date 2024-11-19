#ifndef _SONA_H_
#define _SONA_H_

//#define DEBUG 

#include "Arduino.h"

#define E1 36
#define E2 39
#define E3 34
#define E4 35
#define E5 32
#define E6 33
#define E7 25
#define E8 26
#define E9 27
#define E10 14
#define E11 12

#define T1 23
#define T2 22
#define T3 21

#define I1 0x480
#define I2 0x481
#define I3 0x482
#define I4 0x483
#define I5 0x484
#define I6 0x485
#define I7 0x486
#define I8 0x487
#define I9 0x488
#define I10 0x489
#define I11 0x48A

#define TXpin 1
#define RXpin 3

#define TIMEOUT 40000

/*

이 경우 초음파 센서의 위치에 맞게 CAN 통신 ID 가 지정되어있어야 함.

등화장치와 초음파는 400번대를 함께 씀.
대략 가운데에 있을 수 있도록 전체 번호에서 반을 갈라서 사용.

소리는 어떤 보드에 할당을 해야할까?
상대적으로 시간에 구애를 받지 않는 쪽은 어디인가? 눈으로 보이는 부분은 느려지면 바로 티가 나므로 초음파 쪽에 할당?을 하는 게 맞는 거 같은데...

*/

#define S1_ID 0x480//앞 왼쪽
#define S2_ID 0x481//앞 오른쪽
#define S3_ID 0x482
#define S4_ID 0x483
#define C1_ID 0x48A
#define C2_ID 0x48B
#define C3_ID 0x48C
#define C4_ID 0x48D

#endif
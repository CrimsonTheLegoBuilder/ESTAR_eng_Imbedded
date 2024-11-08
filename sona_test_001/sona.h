#ifndef _SONA_H_
#define _SONA_H_

#include "Arduino.h"

#define S1 39
#define S2 36
#define S3 34
#define S4 35
#define S5 32
#define S6 33
#define S7 25
#define S8 26

/*

이 경우 초음파 센서의 위치에 맞게 CAN 통신 ID 가 지정되어있어야 함.

등화장치와 초음파는 400번대를 함께 씀.
대략 가운데에 있을 수 있도록 전체 번호에서 반을 갈라서 사용.

*/

#define FL_ID 0x480
#define FR_ID 0x481
#define S1_ID 0x482
#define S2_ID 0x483
#define C1_ID 0x48A
#define C2_ID 0x48B
#define C3_ID 0x48C
#define C4_ID 0x48D

#endif
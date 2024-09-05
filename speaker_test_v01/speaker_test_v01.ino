// int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};

// void setup() {
//   for (int i = 0; i < 8; i++) {
//     tone(8, melody[i], 500);
//     delay(500);
//   }
// }

// void loop() {
  
// }

// 음계 주파수 정의 (주요 음들)
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 830
#define NOTE_A5  880

// 멜로디 노트 배열 (간단한 버전)
int melody[] = {
  NOTE_A4, NOTE_CS5, NOTE_E5, NOTE_A5, NOTE_GS5, NOTE_FS5, NOTE_D5, NOTE_A4, NOTE_E5, NOTE_FS5, NOTE_CS5, NOTE_A4,
  NOTE_A4, NOTE_CS5, NOTE_E5, NOTE_A5, NOTE_GS5, NOTE_FS5, NOTE_D5, NOTE_A4, NOTE_E5, NOTE_FS5, NOTE_CS5, NOTE_A4
};

// 각 음의 길이 (단순화를 위해 동일한 길이로 설정)
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

void setup() {
  // 멜로디 재생
  for (int i = 0; i < 24; i++) {
    int duration = 1000 / noteDurations[i];
    tone(8, melody[i], duration);
    delay(duration * 1.3); // 음과 음 사이 약간의 간격 추가
  }
}

void loop() {
  // 추가 동작 없음
}
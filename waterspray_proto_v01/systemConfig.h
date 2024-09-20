#ifndef _SYSTEMCONFIG_H_
#define _SYSTEMCONFIG_H_

//pin map
#define MOTOR1_DIR 26
#define MOTOR1_SPD 27
#define BTN1_PIN 36
#define BTN2_PIN 39
#define PHOTO_PIN 32

#define MOTOR2_DIR 14
#define MOTOR2_SPD 13
#define BTN3_PIN 34
#define BTN4_PIN 35

#define LEVEL_PIN 33
#define PRESSURE_PIN 25

#define RX_PIN 18
#define TX_PIN 17

//dwin address
#define WRITE_DATA 0x82
#define READ_DATA 0x83

//macro
#define FORWARD 1;
#define REVERSE -1;

#define CW 1;
#define CCW -1;

typedef long long ll;
typedef double ld;

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

typedef enum {
  STATE_IDLE,
  STATE_INIT,
  STATE_ROTATE,
  STATE_COMPLETE
} TurntableState_t;

const unsigned long debounce_delay = 200;

const unsigned long pwm_freq = 20000;
const unsigned long pwm_res = 8;

#endif


/*

아래에 있는 코드는 예시로 기록되어있는 코드입니다.

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#define WHAT_THE_FUCK//wtf?

// 세탁기 상태 열거형 정의
typedef enum {
    STATE_IDLE,
    STATE_FILL_WATER,
    STATE_WASHING,
    STATE_DRAIN_WATER,
    STATE_SPINNING,
    STATE_COMPLETE
} WashingMachineState_t;

WashingMachineState_t washerState = STATE_IDLE; // 초기 상태

bool waterLevelReached = false; // 물이 충분히 채워졌는지 여부
bool washComplete = false;      // 세탁이 완료되었는지 여부
bool drainComplete = false;     // 배수가 완료되었는지 여부
bool spinComplete = false;      // 탈수가 완료되었는지 여부

// 세탁기 상태에 따른 태스크
void TaskWashingMachine(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFreq = pdMS_TO_TICKS(100); // 100ms 주기
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        switch (washerState) {
            case STATE_IDLE:
                // 대기 상태: 사용자 입력을 기다림
                Serial.println("Washing Machine is idle...");
                // 예시: 사용자 입력이 감지되면 세탁 시작
                if (userPressedStartButton()) {
                    washerState = STATE_FILL_WATER;
                }
                break;

            case STATE_FILL_WATER:
                // 물을 채우는 상태
                Serial.println("Filling water...");
                fillWater(); // 물 채우기 동작
                if (waterLevelReached) {
                    washerState = STATE_WASHING;
                }
                break;

            case STATE_WASHING:
                // 세탁 상태
                Serial.println("Washing...");
                startWashing(); // 세탁 동작 수행
                if (washComplete) {
                    washerState = STATE_DRAIN_WATER;
                }
                break;

            case STATE_DRAIN_WATER:
                // 배수 상태
                Serial.println("Draining water...");
                drainWater(); // 배수 동작 수행
                if (drainComplete) {
                    washerState = STATE_SPINNING;
                }
                break;

            case STATE_SPINNING:
                // 탈수 상태
                Serial.println("Spinning...");
                startSpinning(); // 탈수 동작 수행
                if (spinComplete) {
                    washerState = STATE_COMPLETE;
                }
                break;

            case STATE_COMPLETE:
                // 완료 상태
                Serial.println("Washing Complete.");
                washerState = STATE_IDLE; // 다시 대기 상태로
                break;
        }

        // 주기적으로 상태를 체크하며 업데이트
        vTaskDelayUntil(&xLastWakeTime, xFreq);
    }
}

// 물이 충분히 채워졌는지 확인하는 함수
bool checkWaterLevel() {
    // 물이 충분히 채워졌는지 센서 값을 읽어 확인
    return (물 채우기 완료 여부);
}

// 사용자 입력 확인 함수
bool userPressedStartButton() {
    // 사용자 입력 버튼을 확인
    return (버튼 눌림 여부);
}

void setup() {
    Serial.begin(9600);

    // 세탁기 상태 관리 태스크 생성
    xTaskCreate(
        TaskWashingMachine,  // 태스크 함수
        "WashingMachine",    // 태스크 이름
        2048,                // 스택 크기
        NULL,                // 매개변수
        2,                   // 우선순위
        NULL                 // 태스크 핸들
    );
}

void loop() {
    // 메인 루프 비어있음 (모든 로직은 FreeRTOS 태스크에서 처리)
}

상태기계 코드

*/
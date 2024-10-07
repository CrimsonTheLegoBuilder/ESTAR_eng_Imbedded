#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <freertos/semphr.h>
#include "EEPROM.h"
#include "systemConfig.h"
#include "motorDriver.h"
// #include "ledControl.h"
// #include "dwinControl.h"

// typedef long long ll;
// typedef double ld;

/*

Arduino Mega PWM pin:
~2 ~ ~13

serial pin
10, 11, 12, 13, 14, 15, 50, 51, 52, 53, 62, 63, 64

Default config::
  RGB_PIN 2 (LED strip)
  DHT_PIN 3
  dwin rx 11
  dwin tx 10
  heatpump 4
  ptc 5
  fan 6

*/

//for ESP32
int memoryPrint() {
    return xPortGetFreeHeapSize();
}

/*
//for Arduino
extern int __bss_end;
extern void *__brkval;

int memoryPrint() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}
*/
const EventBits_t xDisturbanceDetected = 0x01;
SemaphoreHandle_t xSemaphore = NULL;

TurntableState_t TurntableState = STATE_IDLE;
ButtonEvent_t ButtonEvent = EVENT_HOMEBUMP_FIRST;
volatile bool f;

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR motor1CwTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  //motor1_.stop();
  f = 1;
  //timerEnd(timer);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(9600);

  // dht::init(tempValue, humiValue);
  // rgb::init(47);
  // dwin::setup();
  // temper_pid.init(.2, .1, .0005, 1e-4, .0);

  motor1_.begin();
  motor2_.begin();

  motor1_.set_direction(CW);
  motor1_.set_speed(60);
  motor1_.target = PI * 0.666666;

  if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
    xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  Serial.println("start");

  // Serial.println(xTaskCreate(
  //   TaskMotorControl,
  //   "MotorControl",
  //   4096,
  //   NULL,
  //   2,
  //   NULL
  // ));

  Serial.println(xTaskCreate(
    TaskMotorDEBUG,
    "MotorDEBUG",
    4096,
    NULL,
    2,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskRotateMachine,
    "RotateMachine",
    4096,
    NULL,
    2,
    NULL
  ));

  // Serial.println(xTaskCreate(
  //   TaskDhtRead,
  //   "DhtRead",
  //   350,
  //   NULL,
  //   2,
  //   NULL
  // ));

  // Serial.println(xTaskCreate(
  //   TaskDwinRead,
  //   "DwinRead",
  //   150,
  //   NULL,
  //   2,
  //   NULL
  // ));

  // Serial.println(xTaskCreate(
  //   TaskDwinWrite,
  //   "DwinWrite",
  //   150,
  //   NULL,
  //   2,
  //   NULL
  // ));

  // Serial.println(xTaskCreate(
  //   TaskRgbControll,
  //   "RgbControll",
  //   150,
  //   NULL,
  //   1,
  //   NULL
  // ));

  // Serial.println(xTaskCreate(
  //   TaskPidControll,
  //   "PidControll",
  //   150,
  //   NULL,
  //   1,
  //   NULL
  // ));

  // Serial.println(xTaskCreate(
  //   TaskSsrControll,
  //   "SsrControll",
  //   150,
  //   NULL,
  //   1,
  //   NULL
  // ));

  Serial.println("end");

  Serial.println(memoryPrint());
  // vTaskStartScheduler();
  // xCountingSemaphore = xSemaphoreCreateCounting(1,1);
  // xSemaphoreGive(xCountingSemaphore);
}

void loop() {}

// void vDisturbanceDetectionTask(void *pvParameters) {
//     for (;;) {
//         if (detectDisturbance()) {
//             vTaskSuspend(xTaskToControlHandle); // 태스크 일시 정지
//         } else {
//             vTaskResume(xTaskToControlHandle); // 태스크 다시 시작
//         }
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

// void vTaskToControl(void *pvParameters) {
//     for (;;) {
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

void TaskMotorControl(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(50);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      motor1_.count_();
      // motor1_.DEBUG_();
      xSemaphoreGive(xSemaphore);
    }
    // Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

void TaskMotorDEBUG(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(500);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      // motor1_.count_();
      motor1_.DEBUG_();
      xSemaphoreGive(xSemaphore);
    }
    // Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

void TaskRotateMachine(void *pvParameters __attribute__((unused))) {
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(100); // 100ms 주기
  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    switch (TurntableState) {
      case STATE_IDLE:
        Serial.println("preparing to spray...");
        //준비 동작. 압력 채우기 같은 동작들
        /*
        펌프를 동작하는 함수, 분무기 초기화
        */
        if (1) {
          motor1_.set_speed(0);
          delay(2);
          motor1_.set_direction(CCW);
          motor1_.set_speed(150);
          TurntableState = STATE_HOMEBUMP;
        }
        break;

      case STATE_HOMEBUMP:
        Serial.println("spray position init...");
        switch (ButtonEvent) {
          case EVENT_HOMEBUMP_FIRST:
            Serial.println("preparing to spray... EVENT_HOMEBUMP_FIRST");
            if (motor1_.interrupt_flag && digitalRead(motor1_.btn_pin1)) {
              motor1_.interrupt_flag = 0;
              motor1_.direction_changed = false;
              motor1_.set_speed(0);
              delay(2);
              motor1_.set_direction(CW);  // CW로 방향 전환
              timer = timerBegin(1000000);     // 타이머 0, 80분주, true는 카운터 증가
              //timerWrite(timer, 100); // 1000us = 1ms 후 인터럽트 발생
              timerAttachInterrupt(timer, &motor1CwTimer); // 타이머 인터럽트 핸들러 설정
              //timerAlarm(timer, 0, 0, 0);             // 타이머 알람 활성화
              f = 0;
              ButtonEvent = EVENT_ROTATE_CW;
              motor1_.set_speed(100);      // 아주 천천히 CW로 회전
              // timerStart(timer);
              timerAlarm(timer, 2000000, false, 0);
              //timerAlarmEnable(timer);
            }
            break;
          case EVENT_ROTATE_CW:
            Serial.println("preparing to spray... EVENT_ROTATE_CW");
            Serial.print("motor1_.direction_changed: ");
            Serial.println(motor1_.direction_changed);
            Serial.print("f: ");
            Serial.println(f);

            if (motor1_.spd < 10) {
              motor1_.set_direction(CW);  // CW로 방향 전환
              motor1_.set_speed(100);
              break;
            }
            if (f) {
              timerEnd(timer);
              motor1_.direction_changed = false;
              motor1_.set_speed(0);
              delay(2);
              motor1_.set_direction(CCW);  // CW로 방향 전환
              motor1_.set_speed(100);      // 아주 천천히 CW로 회전
              ButtonEvent = EVENT_HOMEBUMP_SECOND;
            }
            break;
          case EVENT_HOMEBUMP_SECOND:
            Serial.println("preparing to spray... EVENT_HOMEBUMP_SECOND");
            if (motor1_.interrupt_flag && digitalRead(motor1_.btn_pin1)) {
              motor1_.interrupt_flag = 0;
              motor1_.cnt = 0;
              ButtonEvent = EVENT_HOMEBUMP_FIRST;
              TurntableState = STATE_ROTATE;
              motor1_.set_speed(150);
            }
            break;
        }
        break;
        //STATE_HOMEBUMP
      case STATE_ROTATE:
        //Serial.println("rotating");

        /*
        
        PID 구현은 일단 다음으로 미루고 생각해보기. 보정까지 고려하면 꽤 오래 걸림. 이 수를 세므로 멈춰야하는 시점까지 얼마나 남았나를 계산 가능함.

        각도 환산 함수가 필요함. cnt를 센 건 그 자체로 각도로 바꿀 수 있고, 마지막으로 출력한 속도 값과 인터럽트로부터 흐른 시간을 토대로 추가로 지나온 각도를 알 수 있음.
        남은 각도로 출력을 어느 정도로 줄이면서 돌려야하는지를 계산.

        */

        /*
        회전 동작 초기:
        PID 계산을 돌린 후 : motor.cal_speed(speed);
        안정적으로 동작하는 속도로 환산한다. -> 일단 출력값을 그대로 각도 변환 명령으로 사용
        방향을 판단한다.
        */

        // switch (ButtonEvent) {
        //   case EVENT_ROTATE_CW:
        //   /*
        //   방향을 판단한 이후의 과정:
        //   방향을 지정한다.
        //   움직이는 함수를 동작한다.
        //   인터럽트가 일어났거나 목표 각도에 동작한 경우? -> 방향을 전환 후 cnt 초기화

        //   필요한 과정: 속도 환산, PID 제어기 출력값 가공.
        //   */
        //   break;

        //   case EVENT_ROTATE_CCW:
        //   break;
        // }

        //Serial.print("rad: ");
        //Serial.println(motor1_.rad(), 6);
        if (motor1_.interrupt_flag && digitalRead(motor1_.btn_pin1)) {
          motor1_.interrupt_flag = 0;
          motor1_.set_speed(0);
          delay(10);
          motor1_.cnt = 0;
          motor1_.rad_ = 0;
          motor1_.set_direction(CW);
          motor1_.set_speed(80);
          break;
        }
        else if (motor1_.rad() >= motor1_.target) {
          motor1_.set_speed(0);
          delay(10);
          motor1_.cnt = 0;
          motor1_.rad_ = 0;
          motor1_.toggle();
          motor1_.set_speed(80);
          break;
        }
        motor1_.set_speed_limit(200, motor1_.rad());
        //Serial.print("speed: ");
        //Serial.println(motor1_.spd);

        break;

      case STATE_COMPLETE:
        Serial.println("Mission Complete.");
        TurntableState = STATE_IDLE; // 다시 대기 상태로
        break;
    }
    // 주기적으로 상태를 체크하며 업데이트
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}


// void TaskDhtRead(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(2000);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       if (t_valid = dht::check(tempValue, humiValue)) {
//         Serial.print("tempValue : ");
//         Serial.println(tempValue);
//         Serial.print("humiValue : ");
//         Serial.println(humiValue);
//       }
//       xSemaphoreGive(xSemaphore);
//     }
//     Serial.println(t_valid);
//     // Serial.println(memoryPrint());
//     vTaskDelayUntil(&xLastWakeTime, xFreq);
//   }
// }

// void TaskDwinWrite(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(2000);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       // Serial.println("Dwin Write");
//       dwin::print_humitemp(tempValue, humiValue);
//       xSemaphoreGive(xSemaphore);
//     }
//     // Serial.println(memoryPrint());
//     vTaskDelayUntil(&xLastWakeTime, xFreq);
//   }
// }

// void TaskDwinRead(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(250);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     TickType_t xS, xE, xExc;
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       // Serial.println(memoryPrint());
//       // Serial.println("Dwin Read");
//       if (dwin::read_setpoint(sp)) Serial.print("new read data: "), Serial.println(sp);
//       else Serial.print("do not read anything. prev read data: "), Serial.println(sp);
//       xSemaphoreGive(xSemaphore);
//     }
//     xE = xTaskGetTickCount();
//     xExc = xE - xS;
//     // Serial.println(memoryPrint());
//     if (xExc < xFreq) vTaskDelayUntil(&xLastWakeTime, xFreq - xExc);
//     else xLastWakeTime = xE;
//     vTaskDelayUntil(&xLastWakeTime, xFreq);
//   }
// }

// void TaskRgbControll(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(250);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       rgb::fixed(sp % 6);
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelayUntil(&xLastWakeTime, xFreq);
//   }
// }

// void TaskPidControll(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(2000);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     TickType_t xS, xE, xExc;
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       // Serial.println("PID_task");
//       if (t_valid) {
//         power = temper_pid.pid_control(sp, tempValue);
//         // Serial.println(t);
//       }
//       // Serial.println(memoryPrint());
//       xSemaphoreGive(xSemaphore);
//     }
//     xE = xTaskGetTickCount();
//     xExc = xE - xS;
//     // if (xExc < xFreq) vTaskDelayUntil(&xLastWakeTime, xFreq - xExc);
//     // else xLastWakeTime = xE;
//     // Serial.println(memoryPrint());
//     vTaskDelayUntil(&xLastWakeTime, xFreq);
//   }
// }

// void TaskSsrControll(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(2000);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     TickType_t xS, xE, xExc;
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       if (prev_button_state != BUTTON_STATE) {
//         ssr::run_dryer(BUTTON_STATE, prev_button_state, power);
//         prev_button_state = BUTTON_STATE;
//       }
//       xSemaphoreGive(xSemaphore);
//     }
//     xE = xTaskGetTickCount();
//     xExc = xE - xS;
//     vTaskDelayUntil(&xLastWakeTime, xFreq);
//   }
// }


//.elf 로그 저장되는 경로
/*

C:\Users\user\AppData\Local\Temp\arduino\sketches\

*/

//PID 예제
        /*
        
            current_position = motor1_.degree();  // 현재 각도 측정

    motorPID.Compute();  // PID 계산

    // 가속도 제어를 통해 급격한 속도 변화를 방지
    if (abs(output_speed - motor1_.spd) > accel_rate) {
        if (output_speed > motor1_.spd) {
            motor1_.set_speed(motor1_.spd + accel_rate);  // 서서히 가속
        } else {
            motor1_.set_speed(motor1_.spd - accel_rate);  // 서서히 감속
        }
    } else {
        motor1_.set_speed(output_speed);  // 이미 가속도가 맞으면 그대로 설정
    }


    #include <Arduino.h>
#include "pidControl.h"
#include "FreeRTOS.h"

float target_position = 180.0;  // 목표 위치
float current_position = 0.0;   // 현재 위치
float output_speed = 0.0;       // PID 계산 후 얻은 속도
float max_speed = 255.0;        // 모터의 최대 속도
float accel_rate = 5.0;         // 가속도 (속도를 올리는 비율)

// PID 객체 선언
PID motorPID(&current_position, &output_speed, &target_position, Kp, Ki, Kd, DIRECT);

// 모터 상태
int motor_speed = 0;            // 현재 모터 속도

// FreeRTOS 태스크 핸들 선언
TaskHandle_t PIDTaskHandle = NULL;
TaskHandle_t SpeedControlTaskHandle = NULL;

// PID 제어 태스크
void PIDTask(void *pvParameters) {
    motorPID.SetMode(AUTOMATIC);  // PID 자동 제어 모드
    motorPID.SetOutputLimits(-max_speed, max_speed);  // PID 출력 값 제한
    motorPID.SetSampleTime(10);  // PID 샘플 타임 설정 (10ms)

    while (1) {
        current_position = motor1_.degree();  // 현재 위치 업데이트
        motorPID.Compute();  // PID 계산
        vTaskDelay(10 / portTICK_PERIOD_MS);  // 10ms마다 실행
    }
}

// 가속도 제어 태스크
void SpeedControlTask(void *pvParameters) {
    while (1) {
        // PID로부터 계산된 출력 속도와 현재 모터 속도의 차이를 비교하여 가속도 제어
        if (abs(output_speed - motor_speed) > accel_rate) {
            if (output_speed > motor_speed) {
                motor_speed += accel_rate;  // 서서히 가속
            } else {
                motor_speed -= accel_rate;  // 서서히 감속
            }
        } else {
            motor_speed = output_speed;  // 가속도 범위 내에서 속도 설정
        }
        motor1_.set_speed(motor_speed);  // 모터 속도 설정
        vTaskDelay(10 / portTICK_PERIOD_MS);  // 10ms마다 실행
    }
}

void setup() {
    // FreeRTOS 태스크 생성
    xTaskCreate(PIDTask, "PID Control Task", 2048, NULL, 1, &PIDTaskHandle);
    xTaskCreate(SpeedControlTask, "Speed Control Task", 2048, NULL, 1, &SpeedControlTaskHandle);
}

void loop() {
    // FreeRTOS는 loop()가 아닌 태스크에서 실행되므로 loop는 비워둡니다.
}

      case STATE_ROTATE:
        Serial.println("rotating");

        
        
        PID 구현은 일단 다음으로 미루고 생각해보기. 보정까지 고려하면 꽤 오래 걸림. 이 수를 세므로 멈춰야하는 시점까지 얼마나 남았나를 계산 가능함.

        각도 환산 함수가 필요함. cnt를 센 건 그 자체로 각도로 바꿀 수 있고, 마지막으로 출력한 속도 값과 인터럽트로부터 흐른 시간을 토대로 추가로 지나온 각도를 알 수 있음.
        남은 각도로 출력을 어느 정도로 줄이면서 돌려야하는지를 계산.

        

        
        회전 동작 초기:
        PID 계산을 돌린 후 : motor.cal_speed(speed);
        안정적으로 동작하는 속도로 환산한다. -> 일단 출력값을 그대로 각도 변환 명령으로 사용
        방향을 판단한다.

        switch (ButtonEvent) {
          case EVENT_ROTATE_CW:
          
          방향을 판단한 이후의 과정:
          방향을 지정한다.
          움직이는 함수를 동작한다.
          인터럽트가 일어났거나 목표 각도에 동작한 경우? -> 방향을 전환 후 cnt 초기화

          필요한 과정: 속도 환산, PID 제어기 출력값 가공.
          
          break;

          case EVENT_ROTATE_CCW:
          break;
        }
        break;


        */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
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

void setup() {
  Serial.begin(9600);

  // dht::init(tempValue, humiValue);
  // rgb::init(47);
  // dwin::setup();
  // temper_pid.init(.2, .1, .0005, 1e-4, .0);

  motor1_.begin();
  motor2_.begin();

  motor1_.set_direction(1);
  motor1_.set_speed(255);

  if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
    xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  Serial.println("start");

  Serial.println(xTaskCreate(
    TaskMotorControl,
    "MotorControl",
    4096,
    NULL,
    2,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskMotorDEBUG,
    "MotorDEBUG",
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
      // motor1_.DEBUG();
      xSemaphoreGive(xSemaphore);
    }
    // Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

void TaskMotorDEBUG(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(50);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      // motor1_.count_();
      motor1_.DEBUG();
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
        if (1) {
          TurntableState = STATE_INIT;
        }
        break;

      case STATE_INIT:
        Serial.println("spray position init...");

        break;

      case STATE_ROTATE:
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

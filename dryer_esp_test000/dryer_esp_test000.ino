#include <Arduino.h>
#include "EEPROM.h"
#include "mathx.h"
#include "ledControl.h"
#include "wtsControl.h"
// #include "pidControl.h"
// #include "ssrControl.h"
// #include "dwinControl.h"
//#include <FreeRTOS.h>
//#include <task.h>
//#include <semphr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

// 전역 변수 및 상수 정의

//namespace wts
float tempValue = 20, humiValue = 20;
bool t_valid = false;
int sp;
Data data;

// bool prev_button_state;
// bool BUTTON_STATE;
// PID temper_pid = PID();
// float power;

SemaphoreHandle_t xSemaphore = NULL;

// 메모리 확인 함수
int memoryPrint() {
  // ESP32에서는 이 방법이 정확하지 않을 수 있습니다. 대신 heap_caps_get_free_size()를 사용할 수 있습니다.
  return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void setup() {
  Serial.begin(9600);

  wts::init(data);
  rgb::init(47);
  sp = 0;
  // dwin::setup();
  // temper_pid.init(.2, .1, .0005, 1e-4, .0);

  xSemaphore = xSemaphoreCreateMutex();
  if (xSemaphore != NULL) {
    xSemaphoreGive(xSemaphore);
  }

  // Task 생성 및 오류 확인
  BaseType_t result;
  result = xTaskCreate(TaskDhtRead, "DhtRead", 2048, NULL, 2, NULL);
  if (result != pdPASS) {
    Serial.println("Failed to create DhtRead task");
  }

  // result = xTaskCreate(TaskDwinRead, "DwinRead", 2048, NULL, 2, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create DwinRead task");
  // }

  // result = xTaskCreate(TaskDwinWrite, "DwinWrite", 2048, NULL, 2, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create DwinWrite task");
  // }

  result = xTaskCreate(TaskRgbControll, "RgbControll", 2048, NULL, 1, NULL);
  if (result != pdPASS) {
    Serial.println("Failed to create RgbControll task");
  }

  // result = xTaskCreate(TaskPidControll, "PidControll", 2048, NULL, 1, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create PidControll task");
  // }

  // result = xTaskCreate(TaskSsrControll, "SsrControll", 2048, NULL, 1, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create SsrControll task");
  // }

  Serial.println("Setup complete");
  Serial.println(memoryPrint());
}

void loop() {
  // 메인 루프는 비어 있습니다. 모든 작업은 FreeRTOS 태스크에서 처리합니다.
}

void TaskDhtRead(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(2000);
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      if (t_valid = wts::check(data)) {
        Serial.print("temperature: ");
        Serial.println(data.temp);
        Serial.print("humidity: ");
        Serial.println(data.humi);
      }
      xSemaphoreGive(xSemaphore);
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

// void TaskDwinWrite(void *pvParameters) {
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//   const TickType_t xFrequency = pdMS_TO_TICKS(2000);
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       dwin::print_humitemp(tempValue, humiValue);
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelayUntil(&xLastWakeTime, xFrequency);
//   }
// }

// void TaskDwinRead(void *pvParameters) {
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//   const TickType_t xFrequency = pdMS_TO_TICKS(250);
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       if (dwin::read_setpoint(sp)) {
//         Serial.print("new read data: ");
//         Serial.println(sp);
//       } else {
//         Serial.print("do not read anything. prev read data: ");
//         Serial.println(sp);
//       }
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelayUntil(&xLastWakeTime, xFrequency);
//   }
// }

void TaskRgbControll(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(250);
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      rgb::rotate(sp % 6);
      xSemaphoreGive(xSemaphore);
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

// void TaskPidControll(void *pvParameters) {
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//   const TickType_t xFrequency = pdMS_TO_TICKS(2000);
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       if (t_valid) {
//         power = temper_pid.pid_control(sp, tempValue);
//       }
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelayUntil(&xLastWakeTime, xFrequency);
//   }
// }

// void TaskSsrControll(void *pvParameters) {
//   TickType_t xLastWakeTime = xTaskGetTickCount();
//   const TickType_t xFrequency = pdMS_TO_TICKS(2000);
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       if (prev_button_state != BUTTON_STATE) {
//         ssr::run_dryer(BUTTON_STATE, prev_button_state, power);
//         prev_button_state = BUTTON_STATE;
//       }
//       xSemaphoreGive(xSemaphore);
//     }
//     vTaskDelayUntil(&xLastWakeTime, xFrequency);
//   }
// }
#include <Arduino.h>
#include "EEPROM.h"
#include "datainfo.h"
#include "ledControl.h"
#include "wtsControl.h"
#include "dwinControl.h"
#include "pidControl.h"
#include "ssrControl.h"
//#include <FreeRTOS.h>
//#include <task.h>
//#include <semphr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

/*
DWIN LCD에서 받은 각 명령은 주소가 할당되어 있고
각 주소들은 datainfo에 enum으로 정리되어 있음.

DWIN LCD의 TX, RX 핀은 2번을 사용.

단순 계산:
우선순위: 낮음 (1)
주기: 100ms

GPIO 출력:
우선순위: 중간 (2)
주기: 10ms

GPIO 입력:
우선순위: 높음 (3)
주기: 10ms

UART 출력:
우선순위: 중간 (2)
주기: 1000ms

UART 입력:
우선순위: 높음 (3)
주기: 가능하면 작게 설정 (1ms)

*/

//variables
// struct Event {
//   EventType type;
//   int val; // 필요한 경우 값 전달
// };

//freeRTOS
SemaphoreHandle_t xSemaphore = NULL;
QueueHandle_t eventQueue;

TaskHandle_t InputTaskHandle = NULL;
TaskHandle_t EventHandlerTaskHandle = NULL;
TaskHandle_t WtsTaskHandle = NULL;
TaskHandle_t RunDryerTaskHandle = NULL;
TaskHandle_t SetTemperatureTaskHandle = NULL;
TaskHandle_t StopDryerTaskHandle = NULL;
TaskHandle_t PIDTaskHandle = NULL;
TaskHandle_t RGBTaskHandle = NULL;

/*
입력을 받는 태스크
태스크를 실행하는 태스크
건조기 태스크
PID 태스크
온도계 태스크
LED 제어 태스크
인터럽트 - 문 열림 인식, 비상 정지

*/

float tempValue = 20, humiValue = 20;
bool t_valid = false;
int sp;
Data data;
Event event;
bool isDryerRunning;
int stop_;

int memoryPrint() {
  return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void setup() {
  Serial.begin(9600);

  isDryerRunning = false;

  wts::init(data);
  rgb::init(47);
  sp = 0;
  stop_ = 0;
  // dwin::setup();
  // temper_pid.init(.2, .1, .0005, 1e-4, .0);

  eventQueue = xQueueCreate(10, sizeof(Event)); // 큐 크기 및 이벤트 크기 설정

  xSemaphore = xSemaphoreCreateMutex();
  if (xSemaphore != NULL) {
    xSemaphoreGive(xSemaphore);
  }

  // Task 생성 및 오류 확인
  BaseType_t result;

  result = xTaskCreate(DwinReadTask, "DwinRead", 2048, NULL, 1, &InputTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create DwinRead task");
  }

  result = xTaskCreate(EventHandlerTask, "EventHandlerTask", 2048, NULL, 1, &EventHandlerTaskHandle);
    if (result != pdPASS) {
    Serial.println("Failed to create event handle task");
  }

  result = xTaskCreate(WtsReadTask, "WtsRead", 2048, NULL, 2, &WtsTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create WtsRead task");
  }

  result = xTaskCreate(RunDryerTask, "RunDryer", 2048, NULL, 1, &RunDryerTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create RunDryer task");
  }

  result = xTaskCreate(StopDryerTask, "StopDryer", 2048, NULL, 1, &StopDryerTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create RunDryer task");
  }

  // result = xTaskCreate(TaskDwinWrite, "DwinWrite", 2048, NULL, 2, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create DwinWrite task");
  // }

  result = xTaskCreate(PIDTask, "PidControll", 2048, NULL, 1, &PIDTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create PID task");
  }

  // result = xTaskCreate(TaskSsrControll, "SsrControll", 2048, NULL, 1, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create SsrControll task");
  // }

  result = xTaskCreate(RgbTask, "Rgb", 2048, NULL, 1, &RGBTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create Rgb task");
  }

  Serial.println("Setup complete");
  Serial.println(memoryPrint());
}

void loop() {}

//========================================================================//
//========================================================================//
//========================================================================//

void DwinReadTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(250);
  Event info;
  for (;;) {
    if (dwin::read(info)) {
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        event.type = info.type;
        event.val = info.val;
        xSemaphoreGive(xSemaphore);
        xQueueSend(eventQueue, &event, portMAX_DELAY);
      }
      //vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void EventHandlerTask(void *pvParameters) {
  Event event;
  for (;;) {
    if (xQueueReceive(eventQueue, &event, portMAX_DELAY)) {
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        switch (event.type) {
          case RUN_DRYER:
            if (!isDryerRunning) {
              isDryerRunning = true;
              xTaskNotifyGive(RunDryerTaskHandle);
              xTaskNotifyGive(PIDTaskHandle);
            }
            break;

          case STOP_DRYER:
            stop_ = event.val;
            xTaskNotifyGive(StopDryerTaskHandle);
            break;

          case SET_TEMPERATURE:
            xTaskNotifyGive(SetTemperatureTaskHandle);
            break;

          case EMERGENCY_STOP:
            isDryerRunning = false;
            xTaskNotifyGive(StopDryerTaskHandle);
            break;
          // 필요에 따라 추가 이벤트 처리
        }

        // 세마포어 해제
        xSemaphoreGive(xSemaphore);
      }
    }
  }
}

void WtsReadTask(void *pvParameters) {
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

void RgbTask(void *pvParameters) {
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

void RunDryerTask(void *pvParameters) {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Starting Dryer...");

    while (true) {
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        if (!isDryerRunning) {

          xSemaphoreGive(xSemaphore);
          break;
        }
        xSemaphoreGive(xSemaphore);
      }

      Serial.println("Running Dryer...");
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    Serial.println("Dryer Stopped.");
  }
}

void StopDryerTask(void *pvParameters) {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Stop");
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      if (stop_ == 2) {
        isDryerRunning = 0;
        //
      }
      xSemaphoreGive(xSemaphore);
    }

    vTaskSuspend(RunDryerTaskHandle); // 태스크 일시 중지
    vTaskSuspend(PIDTaskHandle); // PID 제어 태스크 일시 중지
  }
}

void PIDTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(2000);
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("PID start...");

    while (true) {
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        if (!isDryerRunning) {
          //init PID
          xSemaphoreGive(xSemaphore);
          break;
        }
        xSemaphoreGive(xSemaphore);
      }

      Serial.println("PID processing...");
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    Serial.println("PID Stopped.");
  }
}

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
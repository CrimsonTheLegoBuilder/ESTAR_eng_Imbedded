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

온도계 33 25 26 27

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
TaskHandle_t HeatPumpTaskHandle = NULL;
TaskHandle_t IRheaterTaskHandle = NULL;

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
bool pump_state;
bool heater_state;

int stop_;

wts wts1(TMP_1, HUM_1);
wts wts2(TMP_2, HUM_2);

PID pump(1, 20), heater;

int memoryPrint() {
  return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void setup() {
  Serial.begin(9600);
  Serial.println("setup start");

  isDryerRunning = false;
  pump_state = 0;
  heater_state = 0;
  Serial.println("wts init");
  wts1.init(data);
  wts2.init(data);
  Serial.println("rgb init");
  rgb::init(47);
  dwin::setup();
  sp = 0;
  stop_ = 0;
  // dwin::setup();
  Serial.println("pump, heater init");
  pump.init(.2, .1, .0005, 1e-4, .0);
  heater.init(.2, .1, .0005, 1e-4, .0);
  ssr::init();

  Serial.println("queue");
  eventQueue = xQueueCreate(10, sizeof(Event)); // 큐 크기 및 이벤트 크기 설정

  Serial.println("semaphore");
  xSemaphore = xSemaphoreCreateMutex();
  if (xSemaphore != NULL) {
    xSemaphoreGive(xSemaphore);
  }

  // Task 생성 및 오류 확인
  Serial.println("task");
  BaseType_t result;

  result = xTaskCreate(DwinReadTask, "DwinRead", 2048, NULL, 3, &InputTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create DwinRead task");
  }

  result = xTaskCreate(EventHandlerTask, "EventHandlerTask", 2048, NULL, 3, &EventHandlerTaskHandle);
    if (result != pdPASS) {
    Serial.println("Failed to create event handle task");
  }

  result = xTaskCreate(WtsReadTask, "WtsRead", 2048, NULL, 2, &WtsTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create WtsRead task");
  }

  // result = xTaskCreate(RunDryerTask, "RunDryer", 2048, NULL, 1, &RunDryerTaskHandle);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create RunDryer task");
  // }

  result = xTaskCreate(StopDryerTask, "StopDryer", 2048, NULL, 1, &StopDryerTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create RunDryer task");
  }

  // result = xTaskCreate(TaskDwinWrite, "DwinWrite", 2048, NULL, 2, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create DwinWrite task");
  // }

  // result = xTaskCreate(PIDTask, "PidControll", 2048, NULL, 1, &PIDTaskHandle);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create PID task");
  // }

  // result = xTaskCreate(TaskSsrControll, "SsrControll", 2048, NULL, 1, NULL);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create SsrControll task");
  // }

  // result = xTaskCreate(RgbTask, "Rgb", 2048, NULL, 1, &RGBTaskHandle);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create Rgb task");
  // }

  result = xTaskCreate(HeatPumpTask, "HeatPump", 2048, NULL, 1, &HeatPumpTaskHandle);
  if (result != pdPASS) {
    Serial.println("Failed to create HeatPumpTask task");
  }

  // result = xTaskCreate(IRheaterTask, "IRheater", 2048, NULL, 1, &IRheaterTaskHandle);
  // if (result != pdPASS) {
  //   Serial.println("Failed to create IRheaterTask task");
  // }

  Serial.println("Setup complete");
  Serial.println(memoryPrint());
}

// void loop() { Serial.println("DEBUG::"); }
void loop() {}

//========================================================================//
//========================================================================//
//========================================================================//

void DwinReadTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(250);
  Event info;
  for (;;) {
    Serial.println("dwin::read");
    if (dwin::read(info)) {
      Serial.println("Read something");
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        event.type = info.type;
        event.val = info.val;
        xSemaphoreGive(xSemaphore);
        xQueueSend(eventQueue, &event, portMAX_DELAY);
      }
    }
    Serial.print("info.type:: ");
    Serial.println(info.type);
    Serial.print("info.val:: ");
    Serial.println(info.val);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void EventHandlerTask(void *pvParameters) {
  Event event;
  for (;;) {
    //Serial.println("fucking running event");
    if (xQueueReceive(eventQueue, &event, portMAX_DELAY)) {
      Serial.println("event on");
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        switch (event.type) {
          
          case SET_TEMPERATURE:
            pump.setpoint = event.val;
            heater.setpoint = event.val;
            //xTaskNotifyGive(SetTemperatureTaskHandle);
            break;
            
          case RUN_DRYER:
            if (!isDryerRunning) {
              isDryerRunning = true;
              Serial.println("RUN_DRYER fuck");
              xTaskNotifyGive(HeatPumpTaskHandle);
              //xTaskNotifyGive(IRheaterTaskHandle);
            }
            break;

          case STOP_DRYER:
            // stop_ = event.val;
            stop_ = 2;
            xTaskNotifyGive(StopDryerTaskHandle);
            break;

          case EMERGENCY_STOP:
            isDryerRunning = false;
            ssr::heatpump_control(0);
            ssr::ir_control(0);
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
      if (t_valid = wts1.check(data)) {
        Serial.print("temperature: ");
        Serial.println(data.temp);
        Serial.print("humidity: ");
        Serial.println(data.humi);
      }
      xSemaphoreGive(xSemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(2000 / portTICK_PERIOD_MS));
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

void StopDryerTask(void *pvParameters) {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Stop");
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      switch (stop_) { 
        case 1:
          //pause
          vTaskSuspend(HeatPumpTaskHandle);
          vTaskSuspend(IRheaterTaskHandle);
          break;
       
        case 2:
          //end
          isDryerRunning = 0;
          break;
      }
      xSemaphoreGive(xSemaphore);
    }
  }
}

// void RunDryerTask(void *pvParameters) {
//   HeaterOrder ho;
//   for (;;) {
//     ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//     Serial.println("Starting Dryer...");

//     while (true) {
//       if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
//         PID::compute(data.temp, ho);
//         xSemaphoreGive(xSemaphore);
//         if (!isDryerRunning) break;
//       }

//       //pump on
//       ssr::run_dryer(isDryerRunning);
//       Serial.println("Pump ON");
//       vTaskDelay(pdMS_TO_TICKS(ho.pump_on_duration));

//       //pump off
      
//       Serial.println("Pump OFF");
//       vTaskDelay(pdMS_TO_TICKS(ho.pump_off_duration));
//     }

//     Serial.println("Dryer Stopped.");
//   }
// }

void HeatPumpTask(void *pvParameters) {
  HeaterOrder ho;
  ld tmp;
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Starting Pump...");

    while (true) {
      // if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      //   if (!isDryerRunning) {
      //     ssr::heatpump_control(LOW);
      //     xSemaphoreGive(xSemaphore);
      //     Serial.println("HeatPump DOWN");
      //     break;
      //   }
      //   Serial.println("HeatPump ON");
      //   tmp = data.temp;
      //   ssr::heatpump_control(HIGH);
      //   xSemaphoreGive(xSemaphore);
      //   pump.compute(tmp, ho);
      // }
      // vTaskDelay(pdMS_TO_TICKS(2000 / portTICK_PERIOD_MS));
      
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        tmp = data.temp;
        if (!isDryerRunning) {
          ssr::heatpump_control(0);
          xSemaphoreGive(xSemaphore);
          Serial.println("HeatPump DOWN");
          break;
        }
        xSemaphoreGive(xSemaphore);
        pump.compute(tmp, ho);
      }

      if (!pump_state && tmp < pump.setpoint - pump.hys) {
        //히스테리시스, 설정 온도보다 매우 낮으면 계속 켜둔다
        ssr::heatpump_control(1);
        vTaskDelay(pdMS_TO_TICKS(PUMP_PERIOD));
      }
      else if (pump_state && tmp > pump.setpoint + pump.hys) {
        //히스테리시스, 설정 온도보다 매우 높으면 계속 꺼둔다
        ssr::heatpump_control(0);
        vTaskDelay(pdMS_TO_TICKS(PUMP_PERIOD));
      }
      else {
        //pump on
        ssr::heatpump_control(1);
        Serial.println("Pump ON");
        vTaskDelay(pdMS_TO_TICKS(ho.pump_on_duration));

        //pump off
        ssr::heatpump_control(0);
        Serial.println("Pump OFF");
        vTaskDelay(pdMS_TO_TICKS(ho.pump_off_duration));
      }
      
    }
    Serial.println("Pump Stopped.");
  }
}

void IRheaterTask(void *pvParameters) {
  HeaterOrder ho;
  ld tmp;
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("Starting Heater...");

    while (true) {
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        tmp = data.temp;
        xSemaphoreGive(xSemaphore);
        if (!isDryerRunning) {
          ssr::ir_control(0);
          break;
        }
        heater.compute(tmp, ho);
      }
      ssr::ir_control(ho.ir_heater_pow);
      vTaskDelay(pdMS_TO_TICKS(ho.pump_off_duration));
    }
    Serial.println("heater Stopped.");
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
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include "turntable.h"

int memoryPrint() {
  // ESP32에서는 이 방법이 정확하지 않을 수 있습니다. 대신 heap_caps_get_free_size()를 사용할 수 있습니다.
  return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dwin::setup();

  xSemaphore = xSemaphoreCreateMutex();
  if (xSemaphore != NULL) {
    xSemaphoreGive(xSemaphore);
  }

  BaseType_t result;

  result = xTaskCreate(TaskDwinRead, "DwinRead", 2048, NULL, 2, NULL);
  if (result != pdPASS) {
    Serial.println("Failed to create DwinRead task");
  }

  result = xTaskCreate(TaskDwinWrite, "DwinWrite", 2048, NULL, 2, NULL);
  if (result != pdPASS) {
    Serial.println("Failed to create DwinWrite task");
  }

  Serial.println("Setup complete");
  Serial.println(memoryPrint());
}

void loop() {}

void TaskDhtRead(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(2000);

  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {

      xSemaphoreGive(xSemaphore);
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void TaskDwinWrite(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(2000);

  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {

      xSemaphoreGive(xSemaphore);
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

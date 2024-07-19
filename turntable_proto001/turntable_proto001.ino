#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include "turntable.h"

int memoryPrint() {
  return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void setup() {
  Serial.begin(9600);
  dwin::setup();

  xSemaphore = xSemaphoreCreateMutex();
  if (xSemaphore != NULL) {
    xSemaphoreGive(xSemaphore);
  }

  BaseType_t result;

  result = xTaskCreate(TaskDwinRead, "WTSRead", 2048, NULL, 2, NULL);
  if (result != pdPASS) {
    Serial.println("Failed to create WTSRead task");
  }

  result = xTaskCreate(TaskDwinWrite, "DwinWrite", 2048, NULL, 2, NULL);
  if (result != pdPASS) {
    Serial.println("Failed to create DwinWrite task");
  }

  Serial.println("Setup complete");
  Serial.println(memoryPrint());
}

void loop() {}

void TaskWTSRead(void *pvParameters) {
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

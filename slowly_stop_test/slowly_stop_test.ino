#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>
#include "LEDnode.h"

typedef long long ll;
typedef double ld;

extern int __bss_end;
extern void *__brkval;

int memoryPrint() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}

TaskHandle_t xLedHandle = NULL;
bool isTaskSuspended = false;  // 태스크가 정지되었는지 여부를 추적
bool changed;
int MODE;

const EventBits_t xDisturbanceDetected = 0x01;

void setup() {
  Serial.begin(9600);

  changed = 0;

  if (xSemaphore == NULL) {// Check to confirm that the Serial Semaphore has not already been created.
    xSemaphore = xSemaphoreCreateMutex();// Create a mutex semaphore we will use to manage the Serial Port
    if ((xSemaphore) != NULL) xSemaphoreGive((xSemaphore));// Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  Serial.println("start");

  Serial.println(xTaskCreate(
    TaskSerialRead,
    "SerialRead",
    200,
    NULL,
    2,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskLedlight,
    "Ledlight",
    200,
    NULL,
    2,
    &xLedHandle
  ));

  Serial.println("end");

  Serial.println(memoryPrint());

}

void loop() {}

void TaskSerialRead(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(20);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      if (Serial.available()) {
        MODE = Serial.parseInt();
        changed = 1;
      }
      xSemaphoreGive(xSemaphore);
    }
    // Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

void TaskLedlight(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(20);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      LED::run_led(MODE);
      xSemaphoreGive(xSemaphore);
    }
    // Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

void vDisturbanceDetection(void *pvParameters) {
  for (;;) {
    if (LED::detect_disturbance()) {
      if (!isTaskSuspended) {
        vTaskSuspend(xLedHandle);
        isTaskSuspended = true;
      }
    }
    else {
      if (isTaskSuspended) {
        vTaskResume(xLedHandle);
        isTaskSuspended = false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

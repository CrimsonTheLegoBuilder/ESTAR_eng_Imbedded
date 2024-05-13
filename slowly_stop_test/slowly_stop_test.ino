#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>
#include "LEDnode.h"

#define detector 5

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
int brightness;

int run_state;//1 = working, -1 = slowly decrease, 0 = do not working
int signal_occur;
int stop_order_flag;
int prev_door_state;//0 = opened, 1 = closed
int interrupt_change;
int heater_flag;

const EventBits_t xDisturbanceDetected = 0x01;

void setup() {
  Serial.begin(9600);

  run_state = 1;
  prev_door_state = 0;
  interrupt_change = -1;
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

  attachInterrupt(digitalPinToInterrupt(detector), detect_interrupt, CHANGE);//priority 128?

}

void loop() {}

static void detect_interrupt() {
  if (digitalRead(detector) != prev_door_state) {
    if (prev_door_state == HIGH) {//door open
      prev_door_state = LOW;
      interrupt_change = 1;//door open
      return;
    }
    else {//________________________door close
      prev_door_state = HIGH;
      interrupt_change = 0;//door close
      return;
    }
  }
  return;//no change
}

void TaskSerialRead(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(20);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      if (Serial.available()) {
        MODE = Serial.parseInt();
        signal_occur = 1;
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
      run_state = LED::run_led(MODE, brightness);
      if (!run_state) {
        //open door process
        prev_door_state = 0;
      }
      xSemaphoreGive(xSemaphore);
    }
    // Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

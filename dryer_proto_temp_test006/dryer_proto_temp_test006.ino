#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "dhtControl.h"
#include "dwinControl.h"

typedef long long ll;
typedef double ld;

/*

Default config::
  RGB_PIN 2
  DHT_PIN 3

*/

extern int __bss_end;
extern void *__brkval;

int memoryPrint() {
  int freeMemory;
  if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
  else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
  return freeMemory;
}

int ledPin = 11;
extern int color;
float tempValue = 0, humiValue = 0;
bool t_valid = 0;

SemaphoreHandle_t xSemaphore = NULL;

void setup() {
  Serial.begin(9600);

  dht::init();

  t_valid = 0;

  if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
    xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  Serial.println("start");

  Serial.println(xTaskCreate(
    TaskDhtRead,
    "DhtRead",
    350,
    NULL,
    2,
    NULL
  ));

  Serial.println(xTaskCreate(
    TaskDwinControll,
    "DwinRead",
    350,
    NULL,
    2,
    NULL
  ));

  Serial.println("end");

  Serial.println(memoryPrint());
  // vTaskStartScheduler();
  // xCountingSemaphore = xSemaphoreCreateCounting(1,1);
  // xSemaphoreGive(xCountingSemaphore);
}

void loop() {}

void TaskDhtRead(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(2000);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      // t_valid = dht_sensor.measure(&tempValue, &humiValue);
      auto data = dht::check();
      tempValue = data.temp;
      humiValue = data.humi;
      Serial.print("t_valid : ");
      Serial.println(t_valid);
      Serial.print("tempValue : ");
      Serial.println(tempValue);
      Serial.println(memoryPrint());
      xSemaphoreGive(xSemaphore);
    }
    Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
  }
}

void TaskDwinControll(void *pvParameters __attribute__((unused)) ) {
  // (void) pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFreq = pdMS_TO_TICKS(2000);
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
      Serial.println("Dwin task");
      // ssr::fan_control(50);
      // ssr::ptc_control(50);
      //dwin::print_humitemp(tempValue, humiValue);
      xSemaphoreGive(xSemaphore);
    }
    Serial.println(memoryPrint());
    vTaskDelayUntil(&xLastWakeTime, xFreq);
  }
}

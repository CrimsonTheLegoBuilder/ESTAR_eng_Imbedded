#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include <semphr.h>
#include "EEPROM.h"
// #include "ledControl.h"
// #include "dhtControl.h"
// #include "pidControl.h"
// #include "ssrControl.h"
// #include "dwinControl.h"

typedef long long ll;
typedef double ld;

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

// extern int __bss_end;
// extern void *__brkval;

// int memoryPrint() {
//   int freeMemory;
//   if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
//   else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
//   return freeMemory;
// }

// int ledPin = 11;
// extern int color;
// float tempValue = 20, humiValue = 20;
// bool t_valid = 0;
// int sp;
// PID temper_pid = PID();

SemaphoreHandle_t xSemaphore = NULL;

void setup() {
  // Serial.begin(9600);

  // dht::init(tempValue, humiValue);
  // rgb::init(47);
  // dwin::setup();
  // temper_pid.init(.2, .1, .0005, 1e-4, .0);


  // if ( xSemaphore == NULL ) { // Check to confirm that the Serial Semaphore has not already been created.
  //   xSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
  //   if ( ( xSemaphore ) != NULL ) xSemaphoreGive( ( xSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  // }

  // Serial.println("start");

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

  // Serial.println("end");

  // Serial.println(memoryPrint());
  // vTaskStartScheduler();
  // xCountingSemaphore = xSemaphoreCreateCounting(1,1);
  // xSemaphoreGive(xCountingSemaphore);
}

void loop() {}

// void TaskDhtRead(void *pvParameters __attribute__((unused)) ) {
//   // (void) pvParameters;
//   TickType_t xLastWakeTime;
//   const TickType_t xFreq = pdMS_TO_TICKS(2000);
//   xLastWakeTime = xTaskGetTickCount();
//   for (;;) {
//     if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
//       // Serial.println("fuck");
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
//         float t = temper_pid.pid_control(sp, tempValue);
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
//       // Serial.println("PID_task");
//       ssr::ptc_control(50);
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

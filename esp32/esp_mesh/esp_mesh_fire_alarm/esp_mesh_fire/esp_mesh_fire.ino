//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include <stdio.h>

#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define   MESH_PREFIX     "SKEP-DongSungRoh2"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555
#define   SENSOR_NO       5

#define   RXD_2           4
#define   TXD_2           5
#define   RE_DE           16

#if defined(ESP8266)
 #include <SoftwareSerial.h>
//  SoftwareSerial S(D1, D2, false, 256);

 // receivePin, transmitPin, inverse_logic, bufSize, isrBufSize
 // connect RX to D2 (GPIO4, Arduino pin 4), TX to D1 (GPIO5, Arduino pin 4)
 SoftwareSerial S(4, 5);
#endif

#if   SENSOR_NO == 1
  #define   SERIAL_NO         "d1e4ec25b8c65d3a"
#elif SENSOR_NO == 2
  #define   SERIAL_NO         "bcc1bf7a23189da2"
#elif SENSOR_NO == 3
  #define   SERIAL_NO         "82e7e6a39518b5a1"
#elif SENSOR_NO == 4
  #define   SERIAL_NO         "e2bb3f9f1759b55c"
#elif SENSOR_NO == 5
  #define   SERIAL_NO         "15266a88024cea84"
#elif SENSOR_NO == 6
  #define   SERIAL_NO         "4695cdf09e695d5b"
#elif SENSOR_NO == 7
  #define   SERIAL_NO         "1c54aeb4bfa76f86"
#elif SENSOR_NO == 8
  #define   SERIAL_NO         "53439104097c4cce"

#endif

//#define SERIAL_NO         "d1e4ec25b8c65d3a" // 1
//#define SERIAL_NO         "bcc1bf7a23189da2" // 2
//#define SERIAL_NO         "82e7e6a39518b5a1" // 3
//#define SERIAL_NO         "e2bb3f9f1759b55c" // 4
//#define SERIAL_NO         "15266a88024cea84" // 5
//#define SERIAL_NO         "4695cdf09e695d5b" // 6
//#define SERIAL_NO         "1c54aeb4bfa76f86" // 7
//#define SERIAL_NO         "53439104097c4cce" // 8

#define DEVICE_NO         5
#define DEVICE_TYPE       11 // 10: Buzzer, 11: fire alarm

#define INTERVAL_TIME     9000


#define REGISTER_SIZE     10
#define SLAVE_ID          1

StaticJsonDocument<200> doc;
ModbusRTU mb;

unsigned long loopMillis = 0;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

int timeFlag = 0;

uint16_t registerDatas[REGISTER_SIZE] = {0,};

void setup() {
  Serial.begin(115200);
  #if defined(ESP8266)
    S.begin(9600, SWSERIAL_8N1);
  #elif defined(ESP32)
    Serial2.begin(9600, SERIAL_8N1,RXD_2,TXD_2);
  #endif
  
  initEspMesh();
  modbusSetup();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  sensorLoop();
}

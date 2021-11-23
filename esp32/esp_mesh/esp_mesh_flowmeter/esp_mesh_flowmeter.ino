//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Modbus RTU setting
#define RE_DE             16
#define REGISTER_SIZE     16
#define SLAVE_ID          1
#define RXD_PIN           4
#define TXD_PIN           5

// painlessMesh setting
#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555


// device properties
#define DEVICE_NO         1
#define DEVICE_TYPE       6 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay, 5: vibration, 6: flowmeter

#define INTERVAL_TIME     9000
#define RESET_TIME        300000

StaticJsonDocument<200> doc;

unsigned long loopMillis = 0;

uint16_t reg_vals[REGISTER_SIZE] = {0,};

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

ModbusRTU mb;

int resetFlag = 0;
unsigned long resetMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);
  initEspMesh();
  modbus_setup();
}

void loop() {
  // it will run the user scheduler as well

  mesh.update();
  sensor_loop();
  
}

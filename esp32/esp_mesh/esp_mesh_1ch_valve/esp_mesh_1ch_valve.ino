//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define RELAY_PIN       4
#define CHANNEL         1
#define VALVE_NO        5
#define DEVICE_TYPE     1 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay

#define INTERVAL_TIME   9000
#define FLOW_METER_PIN  5
#define RESET_TIME      300000

StaticJsonDocument<1000> doc;

// FLOW METER VARIABLE
unsigned long flowCount = 0;
unsigned long flowMillis = 0;

int timeFlag = 0;
int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 1;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

int resetFlag = 0;
unsigned long resetMillis = 0;


void pin_setup(){
  pinMode (RELAY_PIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  initEspMesh();
  flow_setup();
  pin_setup();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  valveControll();
  flow_loop();
//  reset_loop();
}

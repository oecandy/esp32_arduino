//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "SKEP-Pentacity"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define RELAY_1_PIN           4 // main valve
#define RELAY_2_PIN           5 // pump
#define CHANNEL               2
#define VALVE_NO              2
#define DEVICE_TYPE           2 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay

#define INTERVAL_TIME         9000
#define FLOW_METER_1_PIN      18
#define FLOW_METER_2_PIN      19
#define RESET_TIME            300000

StaticJsonDocument<200> doc;

// FLOW METER VARIABLE
unsigned long flowCount1 = 0;
unsigned long flowCount2 = 0;
unsigned long flowMillis = 0;

int timeFlag = 0;
int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 1;


int resetFlag = 0;
unsigned long resetMillis = 0;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void pin_setup(){
  pinMode (RELAY_1_PIN, OUTPUT);
  pinMode (RELAY_2_PIN, OUTPUT);
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

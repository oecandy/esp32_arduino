//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include <RCSwitch.h>

#define MESH_PREFIX           "SKEP-DongSungRoh"
#define MESH_PASSWORD         "43100900"
#define MESH_PORT             5555

#define CHANNEL               6
#define VALVE_NO              1
#define DEVICE_TYPE           3 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay

#define INTERVAL_TIME         9000
#define SWING_INTERVAL        8000

#define RESET_TIME            300000
#define SWING_TURN            2000
#define RF_RX_PIN             5

//int test_signal = 0;
//unsigned long testMillis = 0;

StaticJsonDocument<1000> doc;

unsigned long intervalMillis = 0;
unsigned long swingMillis = 0;
int swingFlag = 1;
int turnFlag = 0;
int loop_finish = 0;

int resetFlag = 0;
unsigned long resetMillis = 0;

int timeFlag = 0;
int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 1;
int dir = 0; // 0 : 정지, 1 : left, 2 : right

// RC sensor
RCSwitch mySwitch = RCSwitch();

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

unsigned long code_switch_pan_on_off = 4460136; //decimal RF433 code
unsigned long code_switch_pump_on_off = 4460132; //decimal RF433 code
unsigned long code_switch_lifting_on_off = 4460140; //decimal RF433 code
unsigned long code_switch_right_on_off = 4460130; //decimal RF433 code
unsigned long code_switch_left_on_off = 4460138; //decimal RF433 code
unsigned long code_switch_6 = 4460134;

void setup() {
  Serial.begin(115200);
  initEspMesh();
  RcSensorSetup();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  valveControll();
  interval_loop();
  swing_loop();
}

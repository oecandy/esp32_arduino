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

#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define CHANNEL               1
#define VALVE_NO              1
#define DEVICE_TYPE           10 // 10: Buzzer, 11: fire alarm

#define RESET_TIME            300000
#define INTERVAL_TIME         9000
#define RF_RX_PIN             22
#define SWITCH_PIN            4
#define CONTROL_TIME          10000

StaticJsonDocument<1000> doc;

unsigned long intervalMillis = 0;
unsigned long controlMillis = 0;

int timeFlag = 0;
int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 1;

// RC sensor
RCSwitch mySwitch = RCSwitch();

int resetFlag = 0;
unsigned long resetMillis = 0;
int controlStep = 0;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

//unsigned long codeSwitchBuzzerOn = 1734760; //decimal RF433 code
//#if VALVE_NO == 1
//  // buzzer 1
//  unsigned long codeSwitchBuzzerOn = 1405257944; //decimal RF433 code
//  unsigned long codeMainSwitchBuzzerOn = 1734760; //decimal RF433 code
//#elif VALVE_NO == 2
//  // buzzer 2
//  unsigned long codeSwitchBuzzerOn = 1405257944; //decimal RF433 code
//  unsigned long codeMainSwitchBuzzerOn = 1734756; //decimal RF433 code
//#elif VALVE_NO == 3
//  // buzzer 3
//  unsigned long codeSwitchBuzzerOn = 1405257944; //decimal RF433 code
//  unsigned long codeMainSwitchBuzzerOn = 1734764; //decimal RF433 code
//#elif VALVE_NO == 4
//  // buzzer 4
//  unsigned long codeSwitchBuzzerOn = 1405257944; //decimal RF433 code
//  unsigned long codeMainSwitchBuzzerOn = 1734754; //decimal RF433 code
//#endif

// test
#if VALVE_NO == 1
  // buzzer 1
  unsigned long codeSwitchBuzzerOn = 11049256; //decimal RF433 code
  unsigned long codeMainSwitchBuzzerOn = 1734760; //decimal RF433 code
#elif VALVE_NO == 2
  // buzzer 2
  unsigned long codeSwitchBuzzerOn = 11049252; //decimal RF433 code
  unsigned long codeMainSwitchBuzzerOn = 1734756; //decimal RF433 code
#endif






void setup() {
  Serial.begin(115200);
  pinSetUp();
  initEspMesh();
  RcSensorSetup();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  intervalLoop();
  recieveRcMessage();
  controlLoop();
}

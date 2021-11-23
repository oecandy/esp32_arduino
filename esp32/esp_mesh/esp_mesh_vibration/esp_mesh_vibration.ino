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

//(가속도) 센서 라이브러리
#include<Wire.h>
#include<ADXL345_WE.h>
#define ADXL345_I2CADDR 0x53 // 0x1D if SDO = HIGH
#define ADXL_VCC_PIN    32

//Battery 센서 핀s
#define ADC_PIN 35
#define CONV_FACTOR 1.7
#define READS 20


//(배터리) 센서 라이브러리s
#include <Pangodream_18650_CL.h>

#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define FREQENCY              50 // 50hz
#define COLLECT_COUNT         50 //50회 수집
#define GAIN_VAL              16
#define DEVICE_NO             1
#define DEVICE_TYPE           5 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay, 5: vibration

#define INTERVAL_TIME         5
#define SLEEP_TIME            600
#define uS_TO_S_FACTOR        1000000  /* (딥슬립)Conversion factor for micro seconds to seconds */
#define mS_TO_S_FACTOR        1000
//#define INTERVAL_TIME         3000

StaticJsonDocument<200> doc;
Pangodream_18650_CL BL(ADC_PIN, CONV_FACTOR, READS);

ADXL345_WE myAcc (ADXL345_I2CADDR); // (가속도)센서 주소 지정
RTC_DATA_ATTR int bootCount = 0; // (딥슬립)wakeupCount

unsigned long loopMillis = 0;
float x[COLLECT_COUNT];
float y[COLLECT_COUNT];
float z[COLLECT_COUNT];

int sleepCnt = 0;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  pinMode(ADXL_VCC_PIN, OUTPUT);
  digitalWrite(ADXL_VCC_PIN, HIGH);
//  pinMode(ADXL_VCC2_PIN, OUTPUT);
  initEspMesh();
}

void loop() {
  // it will run the user scheduler as well

  mesh.update();
  sensor_loop();
  
}

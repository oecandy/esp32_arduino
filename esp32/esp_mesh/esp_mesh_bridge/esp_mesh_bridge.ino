#include <WiFi.h>

#define _CRT_SECURE_NO_WARNINGS    // strtok 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>
#include <string.h>    // strtok 함수가 선언된 헤더 파일

// ETH (POE연결 위한) 라이브러리
#include <ETH.h>
#include <PubSubClient.h>

#include "painlessMesh.h"

#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555
#define   RESET_TIME      300000

#define CHANNEL 1
#define maxMqttBufferSize 16384

WiFiClient espClient;
PubSubClient mqttClient(espClient);
StaticJsonDocument<500> doc;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

int resetFlag = 0;
unsigned long resetMillis = 0;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

int restartCnt = 0;

void setup() {
  Serial.begin(115200);
  initEspMesh();  
}

void loop() {
  mesh.update();
//  reset_loop();
}

#include "painlessMesh.h"

#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define CHANNEL 1
#define maxMqttBufferSize 16384

StaticJsonDocument<500> doc;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

bool recvStart = false;
String resultStr = "";

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1,4,5); // olimex poe
  Serial2.setTimeout(100);
  initEspMesh();
}

void loop() {
  mesh.update();
  comunicateSerialData();
}

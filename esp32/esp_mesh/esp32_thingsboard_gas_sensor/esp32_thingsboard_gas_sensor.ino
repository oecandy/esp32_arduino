#include "painlessMesh.h"
#include <WiFi.h>
#include <ThingsBoard.h>
#include "time.h"

#define   MESH_PREFIX       "SKEP-hynix-W12"
#define   MESH_PASSWORD     "43100900"
#define   MESH_PORT         5555

#define   WIFI_AP         "SMARFxSK"
#define   WIFI_PASSWORD   "43100900"
#define   TOKEN           "UsRdS775ZVUqg9gJQoqQ"

painlessMesh  mesh;
struct tm timeinfo;
Scheduler     userScheduler;

char thingsboardServer[] = "192.168.4.1";

WiFiClient wifiClient;

ThingsBoard tb(wifiClient);

StaticJsonDocument<1000> doc;

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup(){
  Serial.begin(115200);
  delay(10000);
  InitWiFi();
//  InitMesh();
  lastSend = 0;
}

void loop(){
  if ( !tb.connected() ) {
    reconnect();
  }
  
//  mesh.update();
  tb.loop();
}

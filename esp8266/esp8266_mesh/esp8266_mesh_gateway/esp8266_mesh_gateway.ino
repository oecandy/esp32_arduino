//************************************************************
// this is a simple example that uses the painlessMesh library to
// connect to a another network and relay messages from a MQTT broker to the nodes of the mesh network.
// To send a message to a mesh node, you can publish it to "painlessMesh/to/12345678" where 12345678 equals the nodeId.
// To broadcast a message to all nodes in the mesh you can publish it to "painlessMesh/to/broadcast".
// When you publish "getNodes" to "painlessMesh/to/gateway" you receive the mesh topology as JSON
// Every message from the mesh which is send to the gateway node will be published to "painlessMesh/from/12345678" where 12345678 
// is the nodeId from which the packet was send.
//************************************************************

#include <Arduino.h>
#include <painlessMesh.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

// Variables about painless mesh info
#define   MESH_PREFIX         "KGC-SeJong"
#define   MESH_PASSWORD       "43100900"
#define   MESH_PORT           5555

// Variables about WiFi info
#define   STATION_SSID        "SMARF_2G"
#define   STATION_PASSWORD    "43100900"

#define   HOSTNAME            "MQTT_Bridge"

// Variables about MQTT Topic & device info
#define   VALVE_1CH_TOPIC     "farota/eth/relay/a0fa7d312314da9c/data"
#define   VALVE_1CH_SUB_TOPIC "farota/eth/relay/a0fa7d312314da9c/cmd"
#define   VALVE_1CH_SERIAL_NO "a0fa7d312314da9c"

// Variables about MQTT connection
#define   MQTT_SERVER         "mqtt.farota.com"
#define   MQTT_PORT           1883

#define maxMqttBufferSize 16384

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
StaticJsonDocument<1000> doc;

IPAddress myIP(0,0,0,0);

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

int restartCnt = 0;

void setup() {
  Serial.begin(115200);
  wifiMqttInit();
  initEspMesh();
}

void loop() {

  mesh.update();
  mqttClient.loop();
  
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
    reconnect();
  }
}

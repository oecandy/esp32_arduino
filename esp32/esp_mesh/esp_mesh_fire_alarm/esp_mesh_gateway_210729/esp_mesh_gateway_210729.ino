#define ETH_CLK_MODE      ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER     12

#include <ETH.h>

static bool eth_connected = false;

#include <PubSubClient.h>

#include "painlessMesh.h"

#define   MESH_PREFIX     "SKEP-DongSungRoh2"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define FIRE_DATA_TOPIC   "farota/eth/fire/"
#define BUZZER_DATA_TOPIC "farota/eth/relay/18b92ceb6bbd1dbf/data"
#define BUZZER_SUB_TOPIC  "farota/eth/relay/18b92ceb6bbd1dbf/cmd"
#define VALVE_1CH_TOPIC "farota/eth/relay/6e6493e954e704b6/data"
#define VIB_DATA_TOPIC "farota/eth/vibration/481c54d2c6ebf0b0/data"
#define VALVE_1CH_SUB_TOPIC "farota/eth/relay/6e6493e954e704b6/cmd"
#define VALVE_2CH_TOPIC     "farota/eth/relay_v3/7db413eaf8645acd/data"
#define VALVE_2CH_SUB_TOPIC "farota/eth/relay_v3/7db413eaf8645acd/cmd"
#define VALVE_6CH_TOPIC     "farota/eth/relay_v5/fa06170632973498/data"
#define VALVE_6CH_SUB_TOPIC "farota/eth/relay_v5/fa06170632973498/cmd"

#define VALVE_1CH_SERIAL_NO "6e6493e954e704b6"
#define VALVE_2CH_SERIAL_NO "7db413eaf8645acd"
#define VALVE_6CH_SERIAL_NO "fa06170632973498"
#define FIRE_SERIAL_NO "839dfe793fad3d01"
#define BUZZER_SERIAL_NO "18b92ceb6bbd1dbf"

#define REBOOT_INTERVAL        3600000

#define CHANNEL 1
#define maxMqttBufferSize 16384

WiFiClient espClient;
PubSubClient mqttClient(espClient);
StaticJsonDocument<1000> doc;

// Variables about MQTT connection
const char* mqttServerIp = "mqtt.farota.com"; // IP address of the MQTT broker
const short mqttServerPort = 1883; // IP port of the MQTT broker
const char* mqttClientName = "booth";
const char* mqttUsername = "farotadev";
const char* mqttPassword = "y!j7DD41-w*uQ!^7";

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

int restartCnt = 0;


void setup() {
  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  
  initEspMesh();
  wifiMqttInit();
}

void loop() {
  // check if ethernet is connected
  if (eth_connected) {
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      mqttClient.loop();
    } 
    // it will run the user scheduler as well
    mesh.update();
  }
}

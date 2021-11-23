// ETH (POE연결 위한) 라이브러리
#include <ETH.h>
#include <ArduinoJson.h>
static bool eth_connected = false;

#include <PubSubClient.h>

#define FIRMWARE_VER      1
#if FIRMWARE_VER == 1

  #define FIRE_DATA_TOPIC     "farota/eth/fire/"
  #define BUZZER_DATA_TOPIC   "farota/eth/relay/18b92ceb6bbd1dbf/data"
  #define BUZZER_SUB_TOPIC    "farota/eth/relay/18b92ceb6bbd1dbf/cmd"
  #define VALVE_1CH_TOPIC     "farota/eth/relay/6e6493e954e704b6/data"
  #define VIB_DATA_TOPIC      "farota/eth/vibration/481c54d2c6ebf0b0/data"
  #define VALVE_1CH_SUB_TOPIC "farota/eth/relay/6e6493e954e704b6/cmd"
  #define VALVE_2CH_TOPIC     "farota/eth/relay_v3/7db413eaf8645acd/data"
  #define VALVE_2CH_SUB_TOPIC "farota/eth/relay_v3/7db413eaf8645acd/cmd"
  #define VALVE_6CH_TOPIC     "farota/eth/relay_v5/fa06170632973498/data"
  #define VALVE_6CH_SUB_TOPIC "farota/eth/relay_v5/fa06170632973498/cmd"
  
  #define VALVE_1CH_SERIAL_NO "6e6493e954e704b6"
  #define VALVE_2CH_SERIAL_NO "7db413eaf8645acd"
  #define VALVE_6CH_SERIAL_NO "fa06170632973498"
  #define BUZZER_SERIAL_NO    "18b92ceb6bbd1dbf"

#elif FIRMWARE_VER == 2

  #define FIRE_DATA_TOPIC     "farota/eth/fire/"
  #define BUZZER_DATA_TOPIC   "farota/eth/relay/buzz_test/data"
  #define BUZZER_SUB_TOPIC    "farota/eth/relay/buzz_test/cmd"
  #define VALVE_1CH_TOPIC     "farota/eth/relay/sprk_test/data"
  #define VIB_DATA_TOPIC      "farota/eth/vibration/vib_test/data"
  #define VALVE_1CH_SUB_TOPIC "farota/eth/relay/sprk_test/cmd"
  #define VALVE_2CH_TOPIC     "farota/eth/relay_v3/2ch_test/data"
  #define VALVE_2CH_SUB_TOPIC "farota/eth/relay_v3/2ch_test/cmd"
  #define VALVE_6CH_TOPIC     "farota/eth/relay_v5/6ch_test/data"
  #define VALVE_6CH_SUB_TOPIC "farota/eth/relay_v5/6ch_test/cmd"
  
  #define VALVE_1CH_SERIAL_NO "sprk_test"
  #define VALVE_2CH_SERIAL_NO "2ch_test"
  #define VALVE_6CH_SERIAL_NO "6ch_test"
  #define BUZZER_SERIAL_NO    "buzz_test"


#endif



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

int restartCnt = 0;

bool recvStart = false;
String resultStr = "";

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1,4,13); // olimex poe
  Serial2.setTimeout(100);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  wifiMqttInit();
}

void loop() {
  // check if ethernet is connected
  
  if (eth_connected) {
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      mqttClient.loop();
      comunicateSerialData();
    } 
  }
}

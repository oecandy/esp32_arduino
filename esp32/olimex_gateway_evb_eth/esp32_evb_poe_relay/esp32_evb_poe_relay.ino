#include <ETH.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define RELAY_1_PIN           32 // main valve
#define RELAY_2_PIN           33 // pump

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정
#define SERIAL_NO             "df52ff092daf4621"

#define INTERVAL_TIME         6000
#define RELAY_TYPE            2

// Update these with values suitable for your network.
const char* ssid = "SMARF";
const char* password = "43100900";
const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* conn_topic = "farota/eth/relay/df52ff092daf4621/connection";
const char* ping_topic = "farota/eth/relay/df52ff092daf4621/ping";
const char* sub_topic = "farota/eth/relay/df52ff092daf4621/cmd";
const char* data_topic = "farota/eth/relay/df52ff092daf4621/data";

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;

static bool ethConnected = false;

unsigned long latestMillis = 0;
int idx_cnt = 0;
char message[200];
int count = 0;

bool send_command = false;

bool command_status[RELAY_TYPE];

int reset_cnt = 0;

void pin_setup(){
  pinMode (RELAY_1_PIN, OUTPUT);
  pinMode (RELAY_2_PIN, OUTPUT);
}

void setup() {
  
  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  pin_setup();
  mqtt_setup();
  
}
void loop() {
  if (ethConnected) {
    if (!client.connected()) {
      reconnect();
    } else {
      client.loop();
      loopValveControll();
      loopPubDeviceData();
    }
  } else {
    delay(1000);
    reset_cnt++;
    if(reset_cnt == 10){
      reset_cnt = 0;
      ESP.restart();
    }
  }
}

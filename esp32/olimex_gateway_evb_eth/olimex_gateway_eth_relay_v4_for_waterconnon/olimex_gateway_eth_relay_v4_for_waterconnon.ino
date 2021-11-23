#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ETH.h>

#define SERIAL_NO             "696658cdd4c5f1e6"    
#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size                
#define INTERVAL_TIME         6000

#define RELAY_PUMP            16 
#define RELAY_FAN             32
#define RELAY_SWING           4
#define RELAY_LIFTING         14
#define RELAY_EXTRA1          13
#define RELAY_EXTRA2          33
#define RELAY_TYPE            6

// CHANGE THESE SETTINGS FOR YOUR APPLICATION
// Update these with values suitable for your network.
const char* ssid = "SMARF";
const char* password = "43100900";
const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* conn_topic = "farota/eth/relay_v4/696658cdd4c5f1e6/connection";
const char* ping_topic = "farota/eth/relay_v4/696658cdd4c5f1e6/ping";
const char* sub_topic = "farota/eth/relay_v4/696658cdd4c5f1e6/cmd";
const char* data_topic = "farota/eth/relay_v4/696658cdd4c5f1e6/data";


// Initializations of network clients
WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1000> doc;

static bool eth_connected = false;

unsigned long latestMillis = 0;
int idx_cnt = 0;
char message[200];
int count = 0;

int reset_cnt = 0;

bool command_status[RELAY_TYPE];

void setup()
{
  Serial.begin(115200);
  pin_setup();
  mqtt_setup();
  setup_eth();
}

void loop(){      
  if (eth_connected) {        // check if ethernet is connected
    if (!client.connected()) {  // now take care of MQTT client...    
      reconnect();
    } else {
      client.loop();
      unsigned long currentMillis = millis();
      if (currentMillis - latestMillis >= INTERVAL_TIME) {
          latestMillis = currentMillis;
          mqtt_publish_data();
      }
    }
  }
  
}

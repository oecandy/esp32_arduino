#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>
#define SERIAL_NO "696658cdd4c5f1e6"    
#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size                
#define INTERVAL_TIME         6000

int RELAY_FOG_ON = 16; 
int RELAY_FOG_OFF = 32;
int RELAY_SWING_ON = 4;  //RXD
int RELAY_SWING_OFF = 14;  //TXD
int RELAY_LIFTING_ON = 13;  //CFG
int RELAY_LIFTING_OFF =33;  //485_EN
//int RELAY_FOG_ON = 2; 
//int RELAY_FOG_OFF = 4;
//int RELAY_SWING_ON = 5;  //RXD
//int RELAY_SWING_OFF = 17;  //TXD
//int RELAY_LIFTING_ON =32;  //CFG
//int RELAY_LIFTING_OFF =33;  //485_EN
int RELAY_TYPE = 6;

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
unsigned long controlMillis = 0;
int idx_cnt = 0;
char message[200];
int count = 0;

int which_cmd;

int reset_cnt = 0;

int relayStatusChange = false;

int control_step = 0;

bool command_status[4];

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
      control_relay(which_cmd);
      unsigned long currentMillis = millis();
      if (currentMillis - latestMillis >= INTERVAL_TIME) {
          latestMillis = currentMillis;
          mqtt_publish_data();
      }
    }
  }
  
}

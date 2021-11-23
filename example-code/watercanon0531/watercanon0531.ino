#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1

#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN 16
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18
#include <ETH.h>
#define SERIAL_NO "waterCannonSerial"    
#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size                
#define INTERVAL_TIME         6000

int RELAY_FOG_ON = 2; 
int RELAY_FOG_OFF = 4;
int RELAY_SWING_ON = 14;  //RXD
int RELAY_SWING_OFF = 15;  //TXD
int RELAY_LIFTING_ON =12;  //CFG
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
const char* conn_topic = "farota/eth/relay/test/connection";
const char* ping_topic = "farota/eth/relay/test/ping";
const char* sub_topic = "farota/eth/relay/test/cmd";
const char* data_topic = "farota/eth/relay/test/data";


// Initializations of network clients
WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1000> doc;

static bool eth_connected = false;

unsigned long latestMillis = 0;
int idx_cnt = 0;
char message[200];
int count = 0;

int which_cmd;

int reset_cnt = 0;

int relayStatusChange = false;

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
      controll_relay(which_cmd);
      unsigned long currentMillis = millis();
      if (currentMillis - latestMillis >= INTERVAL_TIME) {
          latestMillis = currentMillis;
          mqtt_publish_data();
      }
    }
  }
  
}

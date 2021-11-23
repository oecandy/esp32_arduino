#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12


#define INTERVAL_TIME   6000

#include <ETH.h>
#define SERIAL_NO "696658cdd4c5f1e6"    
#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size                

int RELAY[] = {13,14,32,33};
int RELAY_TYPE = 4;

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
DynamicJsonBuffer jsonBuffer;
int idx_cnt = 0;
char message[200];
int count = 0;

bool command_status[4];
static bool eth_connected = false;
unsigned long recentMillis = 0;

void setup()
{
  Serial.begin(115200);
  pin_setup();
  mqtt_setup();
  setup_eth();
}

void loop()
{
  loop_write_coil();
  // check if ethernet is connected  
  if (eth_connected) {
    // now take care of MQTT client...
    if (!client.connected()) {
      reconnect();
    } else {
      //loop_write_coil();
      client.loop();
    }
  }
}

void loop_write_coil(){
  for(int i = 0; i < RELAY_TYPE; i ++){
    controll_valve(i, command_status[i]); 
  }
  unsigned long currentMillis = millis();
  if (currentMillis - recentMillis >= INTERVAL_TIME) {
      recentMillis = currentMillis;
      mqtt_publish_data();
  }
}

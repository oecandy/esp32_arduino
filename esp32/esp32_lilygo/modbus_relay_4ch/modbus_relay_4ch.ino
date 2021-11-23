#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SERIAL_NO "978557fe3ae4917e"

#define RE_DE 19
#define RELAY_TYPE 4
#define SLAVE_ID 1 // 4ch slave id = 1, 8ch slave id = 255

#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size 설정

#define INTERVAL_TIME   6000
#define FLOW_METER_PIN  15 // flow data pin, GPIO 15


// Update these with values suitable for your network.
const char* ssid = "SMARF";
const char* password = "43100900";
const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* conn_topic = "farota/modbus/relay/978557fe3ae4917e/connection";
const char* ping_topic = "farota/modbus/relay/978557fe3ae4917e/ping";
const char* sub_topic = "farota/modbus/relay/978557fe3ae4917e/cmd";
const char* data_topic = "farota/modbus/relay/978557fe3ae4917e/data";

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;
ModbusRTU mb;


int idx_cnt = 0;
char message[200];
int count = 0;

// modbus setup
bool coils[4];
bool valve_status[4];
bool command_status[4];

// FLOW METER VARIABLE
unsigned long flowCount = 0;
unsigned long flowMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,16, 17);
  pin_setup();
  setup_wifi();
  flow_setup();
  modbus_setup();
  mqtt_setup();
}

void loop() {
  flow_loop();
//  loop_modbus();
  loop_write_coil();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

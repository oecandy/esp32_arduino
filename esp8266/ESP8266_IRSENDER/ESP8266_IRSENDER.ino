#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include  <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
#include "c_types.h"
}
#include <Wire.h>
char ssid[] = "eduroam";
char username[] = "smarf";
char identity[] = "smarf";
char password[] = "uiop90-=";

int count = 0;

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;

const char* mqtt_server = "iot-mqtt.farota.com";
const char* user = "sk-pangyo";
const char* pass = "sk-pangyo";

const char* pub_topic = "farota/wifi/ir/dcc01/SYStoMQTT";
const char* sub_topic = "farota/wifi/ir/dcc01/#";
const char* cmd_topic = "farota/wifi/ir/dcc01/cmd/MQTTtoIR";

const uint16_t kIrLed = 14;

IRsend irsend(kIrLed);

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  strcpy((char*)wifi_config.password, password);

  wifi_station_set_config(&wifi_config);

  wifi_station_set_wpa2_enterprise_auth(1);

  // Clean up to be sure no old data is still inside
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_clear_enterprise_identity();
  wifi_station_clear_enterprise_username();
  wifi_station_clear_enterprise_password();
  wifi_station_clear_enterprise_new_password();
  
  wifi_station_set_enterprise_identity((uint8*)identity, strlen(identity));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen((char*)password));

  
  wifi_station_connect();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count>=10){ //after 5 seconds timeout - reset wifi
      count = 0;
      WiFi.reconnect();
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
}

uint64_t stringToBinary (const char* hexString)
{
  uint64_t n;
  sscanf(hexString, "%lx", &n);
  return n;
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(topic);
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
    if(String(topic).equals(String(cmd_topic))){
      irsend.sendNEC(stringToBinary(root["hex"]));
    }
  } else {
    Serial.println("parseObject() failed");
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "IRSENDER-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), user, pass)) {
//    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pub_topic, "connected");
      // ... and resubscribe
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1);
  setup_wifi();
  client.setServer(mqtt_server, 11883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

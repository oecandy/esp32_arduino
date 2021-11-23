#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_wpa2.h"
#include <Wire.h>
#define EAP_IDENTITY "smarf" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "uiop90-=" //your Eduroam password
#define EAP_ANONYMOUS_IDENTITY "smarf" // your anonymous Identity
const char* ssid = "eduroam"; // Eduroam SSID
//const char* ssid = "SMARF";
//const char* password = "43100900";

int count = 0;

//WiFiClientSecure espClient;
WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;

const char* mqtt_server = "iot-mqtt.farota.com";
const char* username = "sk-pangyo";
const char* pass = "sk-pangyo";

const char* pub_topic = "farota/wifi/ir/dcc01/SYStoMQTT";
const char* sub_topic = "farota/wifi/ir/dcc01/#";

const uint16_t kIrLed = 15;

IRsend irsend(kIrLed);

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); // If you have not set the AnoID, Comment out this line!!
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  WiFi.begin(ssid); //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count>=10){ //after 5 seconds timeout - reset wifi //      ESP.restart();
      count = 0;
      WiFi.reconnect();
    }
  }
//  client.setCACert(test_root_ca);
  //client.setCertificate(test_client_key); // for client verification
  //client.setPrivateKey(test_client_cert);  // for client verification
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
}

//void setup_wifi() {
//
//  delay(10);
//  // We start by connecting to a WiFi network
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//    if(count < 5){
//      count ++;
//    } else {
//      count= 0;
//      WiFi.reconnect();
//    }
//  }
//
//  randomSeed(micros());
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//}

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
    irsend.sendNEC(stringToBinary(root["hex"]));
  delay(2000);
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
    if (client.connect(clientId.c_str(), username, pass)) {
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
  
//  Serial.println("a rawData capture from IRrecvDumpV2");
//  irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
//  delay(2000);
//  Serial.println("a Samsung A/C state from IRrecvDumpV2");
//  irsend.sendSamsungAC(samsungState);
//  delay(2000);
}

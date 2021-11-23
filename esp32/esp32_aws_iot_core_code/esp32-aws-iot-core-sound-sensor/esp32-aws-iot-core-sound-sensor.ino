#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ModbusRTU.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "WiFi.h"
#include <NTPClient.h>

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "$aws/rules/ingest/honglim"
#define AWS_IOT_SUBSCRIBE_TOPIC "smarf/sub"
#define INTERVAL_TIME           1000
#define SENSOR_COLL_CNT         300

#define RE_DE                   16
#define RXD_PIN                 4
#define TXD_PIN                 5
#define SOUND_SLAVE_ID          1
#define REGISTER_SIZE           10
#define THINGS_NAME             "sip_honglim-aws_hls-fnd-02_fnd02423"

StaticJsonDocument<1000> doc;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

unsigned long loop_millis = 0;

unsigned long interval_mills = INTERVAL_TIME;

float sound_sensor_datas[SENSOR_COLL_CNT] = {0,};
uint16_t reg_vals[REGISTER_SIZE] = {0, };

int coll_cnt = 0;

unsigned long coll_timestatmp = 0;

typedef struct struct_message {
  float leq;
  float l10;
  float lmax;
} struct_message;
struct_message result;

struct time_delay {
  char name;
  unsigned long value;
};

ModbusRTU mb;


void setup() {
  Serial.begin(115200);
  Serial2.begin(4800, SERIAL_8N1, RXD_PIN, TXD_PIN);
  connectAWS();
  modbusSetup();
  timeClient.begin();
  get_delay_set();
}

void loop() {
  
  client.loop();
  delay(10);
//  Serial.println(client.connected());
  if(!client.connected()) {
//    Serial.println("disconnected mqtt");
  }
  
  sensorLoop();
//  Serial.println(client.connected());
}

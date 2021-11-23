#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Modbus RTU setting
#define RE_DE             16
#define REGISTER_SIZE     2
#define WQ_SLAVE_ID       1
#define WL_SLAVE_ID       2
#define RXD_PIN           4
#define TXD_PIN           5
#define DEVICE_NO         1

#define INTERVAL_TIME     6000

const uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x22, 0x03, 0x28};

esp_now_peer_info_t peerInfo;

StaticJsonDocument<1000> doc;

unsigned long loopMillis = 0;

typedef struct struct_message {
  int device_no;
  uint16_t water_level;
  float water_quality;
} struct_message;
struct_message myData;

uint16_t reg_wq_vals[REGISTER_SIZE] = {0,};
uint16_t reg_wl_vals[REGISTER_SIZE] = {0,};

ModbusRTU mb;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);
  initESPNow();
  modbus_setup();
  
}

void loop() {
  sensor_loop();

}

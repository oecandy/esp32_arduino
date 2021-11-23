#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL               2
#define DEVICE_NO             2
#define SENSOR_TYPE           2 // 1 : hall sensor 2 : laser sensor

#define INTERVAL_TIME         6000

#define LASER_DETECT_PIN 32

// a8:03:2a:a1:34:c8

uint8_t broadcastAddress[] = {0xA8, 0x03, 0x2A, 0xA1, 0x34, 0xC8};

esp_now_peer_info_t peerInfo;

//values for esp_now interval
unsigned long sendIntervalMillis = 0;
uint8_t timeFlag = 0;

/*
 * - sensor data
 * hall sensor : 0 >= sensor [닫힘]
 *               0 < sensor  [열림]
 * laser sensor :  0 [떨어짐]
 *                 1 [붙음]
*/
int sensorData = 0;
int sensorState = 0;
int relayStatus[CHANNEL] = {0,};

typedef struct struct_message {
  int device_no;
  int sensor_type; // 1: hall sensor 2: laser sensor
  int sensor_data;
  int sensor_state;
  int relay_state[CHANNEL];
} struct_message;
struct_message deviceData;

typedef struct recv_message {
  int valve_no;
  int switch_no;
  int relay_state;
} recv_message;
recv_message recvData;

void setup(){
  Serial.begin(115200);
  pinMode (LASER_DETECT_PIN, INPUT);
  initESPNow();
}

void loop() {
  sensorData = digitalRead(LASER_DETECT_PIN); // HIGH(1) = 떨어짐, LOW(0) = 붙음
  if(sensorData){
    sensorState = 0;
  } else {
    sensorState = 1;
  }
//  Serial.print("Detected something: ");
//  Serial.println(sensorData);
  unsigned long currentMillis = millis();
  if (currentMillis - sendIntervalMillis >= INTERVAL_TIME) {
      sendIntervalMillis = currentMillis;
      sendPayload();
  }
}

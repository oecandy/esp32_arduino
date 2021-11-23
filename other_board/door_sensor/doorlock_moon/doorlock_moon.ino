#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL               2
#define DEVICE_NO             1
#define SENSOR_TYPE           1 // 1 : hall sensor 2 : laser sensor

#define RELAY_1_PIN           32
#define RELAY_2_PIN           33

#define THRES_VAL             0

//Setting for Siren
#define BEEP_TIME             2000
#define BEEP_INTERVAL         1000 //secs   --- --- --- --- --- (531), -- -- -- (321)
#define SIREN_REPEAT          15 //repeats for siren

#define DOOR_OPEN_TIME        30000

#define INTERVAL_TIME         6000

// a8:03:2a:a1:34:c8

uint8_t broadcastAddress[] = {0xA8, 0x03, 0x2A, 0xA1, 0x34, 0xC8};

esp_now_peer_info_t peerInfo;

//values can be quite low. 
int val = 0;
int locked = 0;

//values for esp_now interval
unsigned long sendIntervalMillis = 0;
uint8_t timeFlag = 0;

//Siren
unsigned long sirenIntervalMillis = 0;
int repeatCnt = 0;
int sirenStep = 0;

//Door
unsigned long doorIntervalMillis = 0;
int doorStep = 0;

/*
 * - sensor data
 * hall sensor : 0 >= sensor [닫힘]
 *               0 < sensor  [열림]
 * laser sensor :  0 [떨어짐]
 *                 1 [붙음]
*/

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
  pinSetup();
  initESPNow();
  
}

void loop() {
  loopSensor();
  loopSirenDevice();
  loopDoorDevice();
  loopSensorMessage();
  
}

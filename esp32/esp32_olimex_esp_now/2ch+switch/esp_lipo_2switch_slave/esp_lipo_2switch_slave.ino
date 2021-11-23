#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x22, 0x03, 0x28};

esp_now_peer_info_t peerInfo;

#define RELAY_1_PIN           4 // main valve
#define RELAY_2_PIN           5 // pump

#define SWITCH_1_PIN          17
#define SWITCH_2_PIN          16

#define CHANNEL               2
#define VALVE_NO              1
#define DEVICE_TYPE           2

int relayStatus[CHANNEL] = {0,};
int recentSwtich[CHANNEL] = {0,};
int changeSwitch = 0;

int relayStatusChange = false;

int bootSwitch = true;

uint8_t timeFlag = 0;

typedef struct struct_message {
  int valve_no;
  int device_type;
  int switch_no;
  int relay_state[CHANNEL];
  int flow_count[CHANNEL];
} struct_message;
struct_message myData;

typedef struct recv_message {
  int valve_no;
  int switch_no;
  int relay_state;
} recv_message;
recv_message recvData;

void pin_setup(){
  pinMode (SWITCH_1_PIN, INPUT_PULLUP);
  pinMode (SWITCH_2_PIN, INPUT_PULLUP);
}

void setup() {
  Serial.begin(115200);
  initESPNow(); // Init ESPNow with a fallback logic : 초기화
  pin_setup();
}

void loop() {
  checkSwitchStatus();
  
  if(relayStatusChange)
  { 
    sendPayload();
    relayStatusChange = false;
  }
}

#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x22, 0x03, 0x28};

esp_now_peer_info_t peerInfo;

#define RELAY_1_PIN           4 // main valve
#define RELAY_2_PIN           5 // pump

#define CHANNEL               2
#define VALVE_NO              1
#define DEVICE_TYPE           1

#define INTERVAL_TIME         6000
#define FLOW_METER_1_PIN      18
#define FLOW_METER_2_PIN      19

// FLOW METER VARIABLE
unsigned long flowCount1 = 0;
unsigned long flowCount2 = 0;
unsigned long flowMillis = 0;

int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 1;

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
  pinMode (RELAY_1_PIN, OUTPUT);
  pinMode (RELAY_2_PIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  initESPNow(); // Init ESPNow with a fallback logic : 초기화
  flow_setup();
  pin_setup();
}

void loop() {
  if(relayStatusChange)
  { 
    valveControll();
    relayStatusChange = false;
  }
  
  flow_loop();
}

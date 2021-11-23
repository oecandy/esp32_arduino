#include <esp_now.h>
#include <WiFi.h>

#define RELAY_PIN       4
#define CHANNEL         1
#define VALVE_NO        5

#define INTERVAL_TIME   4000
#define FLOW_METER_PIN  5

const uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x20, 0x55, 0x40};

esp_now_peer_info_t peerInfo;

// FLOW METER VARIABLE
unsigned long flowCount = 0;
unsigned long flowMillis = 0;

int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 1;

int timeFlag = 0;

typedef struct struct_message {
  int valve_no;
  int relay_state[CHANNEL];
  int flow_count[CHANNEL];
} struct_message;
struct_message myData;

typedef struct recv_message {
  int valve_no;
  int relay_state;
} recv_message;
recv_message recvData;

void pin_setup(){
  pinMode (RELAY_PIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  initESPNow(); // Init ESPNow with a fallback logic : 초기화
  flow_setup();
  pin_setup();
}

void loop() {
  valveControll();
  flow_loop();
}

#include "painlessMesh.h"       //Mesh 라이브러리
#include <RCSwitch.h>           //433 라이브러리

#define   MESH_PREFIX     "SKEP-DongSungRoh"
#define   MESH_PASSWORD   "43100900"
#define   MESH_PORT       5555

#define RF_RX_PIN       32
#define CHANNEL         1
#define BUTTON_NO        2
#define DEVICE_TYPE     11 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay, 11: 1ch button, 12: 2ch button
                           // 2자리: 앞자리 장치종류, 뒷자리 채널수 

StaticJsonDocument<200> doc;

int timeFlag = 0;
int relayStatus[CHANNEL] = {0,};
int relayStatusChange = false;
int switchNo = 2;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

RCSwitch mySwitch = RCSwitch()

unsigned long code_switchTrenchOn = 1234567890ABCDEFG; //decimal RF433 code
unsigned long code_switchTrenchCodeOff = 9876543210GFEDCBA; //decimal RF433 code
unsigned long code_switchPopupCodeOn = 0123456789ABCDEFG; //decimal RF433 code
unsigned long code_switchPopupCodeOff = 0987654321GFEDCBA; //decimal RF433 code

void sendMessage(4가지 커맨드 구분자) {
  String msg = 구분자;
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(RF_RX_PIN);
  initEspMesh();
}

void loop() {
  // it will run the user scheduler as well
  if (mySwitch.available()){
    if (mySwitch.getReceivedValue()==code_switchTrenchOn){
      sendMessage(구분자 11);   // 받은 브릿지에서는 mqttClient.publish(토픽, 트렌치On커맨드);
      Serial.printf("Trench spray On", from, msg.c_str());
    } else if (mySwitch.getReceivedValue()==code_switchTrenchCodeOff){
      sendMessage(구분자 10);   // 받은 브릿지에서는 mqttClient.publish(토픽, 트렌치On커맨드);
      Serial.printf("Trench spray Off", from, msg.c_str());
    } else if (mySwitch.getReceivedValue()==code_switchPopupCodeOn{
      sendMessage(구분자 21);   // 받은 브릿지에서는 mqttClient.publish(토픽, 트렌치On커맨드);
      Serial.printf("Popup spray On", from, msg.c_str());
    } else if (mySwitch.getReceivedValue()==code_switchPopupCodeOff){
      sendMessage(구분자 20);   // 받은 브릿지에서는 mqttClient.publish(토픽, 트렌치On커맨드);
      Serial.printf("Popup spray Off", from, msg.c_str());
    }
  }
  mesh.update();
}

/*

  Generics based example of rBASE64 Library

  This example shows the calling convention for the various functions.

  For more information about this library please visit us at
  http://github.com/boseji/BASE64

  Created by Abhijit Bose (boseji) on 22/02/16.
  Copyright 2016 - Under creative commons license 3.0:
        Attribution-ShareAlike CC BY-SA

  @version API 1.1.0
  @author boseji - salearj@hotmail.com

*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <rBase64.h>


#define MAX_IN    13
#define SELECT_NUM  12


// ---------------------------------------------------------------------------------
// Service Set IDentifier & Serial Number
const char* ssid = "SMARF";
// const char* ssid = "SMARF_RAK";
const char* password = "43100900";
const char* mqtt_server = "lora.farota.com";

const char* devEUI = "b2a57934dceef69f";
// const char* devEUI1 = "2dcbfa8a7d1dfac7";
// const char* devEUI2 = "069383228e4ed7bd";
// const char* devEUI3 = "74be59f903c55850";
const char* devEUI11 = "910470b2f636f681";
const char* devEUI12 = "e18a7fa2a0dd9544";
const char* devEUI13 = "ad7143eaf8fcf44f";
const char* devEUI14 = "721d393941206868";
const char* devEUI15 = "a988de2d0a72229e";
const char* devEUI16 = "450334cb5681aa88";
const char* devEUI17 = "fc333262fb78eef8";
const char* devEUI18 = "be107069afcb850c";

const char* UpstreamTopic = "application/1/device/b2a57934dceef69f/command/down";   // OTTA-Test
const char* DownstreamTopic = "application/1/device/b2a57934dceef69f/event/up"; // OTTA-Test
const char* UpstreamTopic100 = "application/3/device/0000e0000e0000e0/command/down";  // Sonoff SV Ver 1.0
const char* DownstreamTopic100 = "application/3/device/0000e0000e0000e0/event/up";  // Sonoff SV Ver 1.0
// const char* UpstreamTopic = "application/1/device/b2a57934dceef69f/tx";    // OTTA-Test
// const char* DownstreamTopic = "application/1/device/b2a57934dceef69f/rx";  // OTTA-Test
// const char* UpstreamTopic100 = "application/3/device/0000e0000e0000e0/tx"; // Sonoff SV Ver 1.0
// const char* DownstreamTopic100 = "application/3/device/0000e0000e0000e0/rx"; // Sonoff SV Ver 1.0
// const char* UpstreamTopic1 = "application/3/device/2dcbfa8a7d1dfac7/tx"; // ginseng-ginsengcorperation-pesticide-valve-otaa-01
// const char* DownstreamTopic1 = "application/3/device/2dcbfa8a7d1dfac7/rx"; // ginseng-ginsengcorperation-pesticide-valve-otaa-01
// const char* UpstreamTopic2 = "application/3/device/069383228e4ed7bd/tx"; // ginseng-ginsengcorperation-pesticide-valve-otaa-02
// const char* DownstreamTopic2 = "application/3/device/069383228e4ed7bd/rx"; // ginseng-ginsengcorperation-pesticide-valve-otaa-02
// const char* UpstreamTopic3 = "application/3/device/74be59f903c55850/tx"; // ginseng-ginsengcorperation-pesticide-valve-otaa-03
// const char* DownstreamTopic3 = "application/3/device/74be59f903c55850/rx"; // ginseng-ginsengcorperation-pesticide-valve-otaa-03
const char* UpstreamTopic11 = "application/3/device/910470b2f636f681/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-01
const char* DownstreamTopic11 = "application/3/device/910470b2f636f681/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-01
const char* UpstreamTopic12 = "application/3/device/e18a7fa2a0dd9544/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-02
const char* DownstreamTopic12 = "application/3/device/e18a7fa2a0dd9544/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-02
const char* UpstreamTopic13 = "application/3/device/ad7143eaf8fcf44f/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-03
const char* DownstreamTopic13 = "application/3/device/ad7143eaf8fcf44f/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-03
const char* UpstreamTopic14 = "application/3/device/721d393941206868/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-04
const char* DownstreamTopic14 = "application/3/device/721d393941206868/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-04
const char* UpstreamTopic15 = "application/3/device/a988de2d0a72229e/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-05
const char* DownstreamTopic15 = "application/3/device/a988de2d0a72229e/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-05
const char* UpstreamTopic16 = "application/3/device/450334cb5681aa88/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-06
const char* DownstreamTopic16 = "application/3/device/450334cb5681aa88/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-06
const char* UpstreamTopic17 = "application/3/device/fc333262fb78eef8/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-07
const char* DownstreamTopic17 = "application/3/device/fc333262fb78eef8/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-07
const char* UpstreamTopic18 = "application/3/device/be107069afcb850c/command/down"; // ginseng-ginsengcorperation-water-valve-otaa-08
const char* DownstreamTopic18 = "application/3/device/be107069afcb850c/event/up"; // ginseng-ginsengcorperation-water-valve-otaa-08
// const char* UpstreamTopic11 = "application/3/device/910470b2f636f681/tx";  // ginseng-ginsengcorperation-water-valve-otaa-01
// const char* DownstreamTopic11 = "application/3/device/910470b2f636f681/rx";  // ginseng-ginsengcorperation-water-valve-otaa-01
// const char* UpstreamTopic12 = "application/3/device/e18a7fa2a0dd9544/tx";  // ginseng-ginsengcorperation-water-valve-otaa-02
// const char* DownstreamTopic12 = "application/3/device/e18a7fa2a0dd9544/rx";  // ginseng-ginsengcorperation-water-valve-otaa-02
// const char* UpstreamTopic13 = "application/3/device/ad7143eaf8fcf44f/tx";  // ginseng-ginsengcorperation-water-valve-otaa-03
// const char* DownstreamTopic13 = "application/3/device/ad7143eaf8fcf44f/rx";  // ginseng-ginsengcorperation-water-valve-otaa-03
// const char* UpstreamTopic14 = "application/3/device/721d393941206868/tx";  // ginseng-ginsengcorperation-water-valve-otaa-04
// const char* DownstreamTopic14 = "application/3/device/721d393941206868/rx";  // ginseng-ginsengcorperation-water-valve-otaa-04
// const char* UpstreamTopic15 = "application/3/device/a988de2d0a72229e/tx";  // ginseng-ginsengcorperation-water-valve-otaa-05
// const char* DownstreamTopic15 = "application/3/device/a988de2d0a72229e/rx";  // ginseng-ginsengcorperation-water-valve-otaa-05
// const char* UpstreamTopic16 = "application/3/device/450334cb5681aa88/tx";  // ginseng-ginsengcorperation-water-valve-otaa-06
// const char* DownstreamTopic16 = "application/3/device/450334cb5681aa88/rx";  // ginseng-ginsengcorperation-water-valve-otaa-06
// const char* UpstreamTopic17 = "application/3/device/fc333262fb78eef8/tx";  // ginseng-ginsengcorperation-water-valve-otaa-07
// const char* DownstreamTopic17 = "application/3/device/fc333262fb78eef8/rx";  // ginseng-ginsengcorperation-water-valve-otaa-07
// const char* UpstreamTopic18 = "application/3/device/be107069afcb850c/tx";  // ginseng-ginsengcorperation-water-valve-otaa-08
// const char* DownstreamTopic18 = "application/3/device/be107069afcb850c/rx";  // ginseng-ginsengcorperation-water-valve-otaa-08
// ---------------------------------------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
uint32_t lastMsg = 0;
char msg[550];
char data[50];

byte inputStep = 1;

byte chatSW1 = 1, chatSW2 = 1, chatSW3 = 1, chatSW4 = 1, chatSW5 = 1;
byte chatSW6 = 0, chatSW7 = 0, chatSW8 = 0, chatSW9 = 0, chatSW10 = 0;
byte chatSW11 = 1, chatSW12 = 1, chatSW13 = 1, chatSW14 = 1, chatSW15 = 1;
byte chatSW16 = 0, chatSW17 = 0, chatSW18 = 0, chatSW19 = 0, chatSW20 = 0;
byte chatSW21 = 1, chatSW22 = 1;
byte chatSW26 = 0, chatSW27 = 0;
byte switchIN[MAX_IN] = {0,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1};   // HIGH = ON, LOW = OFF
byte sBackupIN[MAX_IN] = {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0};

unsigned long previousms[8] = {0,  0, 0, 0,  0,  0, 0, 0};
unsigned long currentms[8] = {0,  0, 0, 0,  0,  0, 0, 0};

byte selectTopic = 0;
byte txValveState[21] = {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0};
byte rxValveState[21] = {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0};
byte txFlag[21] = {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0};
unsigned long txTime[21] = {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0};


/***************************************************************************************************************
*
* User Functions
*
***************************************************************************************************************/
/* Input Output port extension setup ------------------------------------------------------------------------ */
void pins_setup (void)
{
  // 74HC138, 74HC238 제어하는 pins 설정
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void SET_inputMode (void)
{
  // change input mode : IO15, IO02, IO00, IO04, IO05
  pinMode(15, INPUT);
  pinMode(02, INPUT);
  pinMode(00, INPUT);
  pinMode(04, INPUT);
  pinMode(05, INPUT);
}

void SET_outputMode (void)
{
  // change output mode : IO15, IO02, IO00, IO04, IO05
  pinMode(15, OUTPUT);
  pinMode(02, OUTPUT);
  pinMode(00, OUTPUT);
  pinMode(04, OUTPUT);
  pinMode(05, OUTPUT);
}

void IC_74HC138D_EN (byte selOut)
{
  switch(selOut)  // 입력 받은 값으로 74HC138D Yx 출력 선택
  {
    case 0:   // 74HC138D Y0 동작
      digitalWrite(16, LOW);  // 74HC138D_A0
      digitalWrite(14, LOW);  // 74HC138D_A1
      break;

    case 1:   // 74HC138D Y1(LOW_EN3) 동작
      digitalWrite(16, HIGH); // 74HC138D_A0
      digitalWrite(14, LOW);  // 74HC138D_A1
      break;

    case 2:   // 74HC138D Y2(LOW_EN2) 동작
      digitalWrite(16, LOW);  // 74HC138D_A0
      digitalWrite(14, HIGH); // 74HC138D_A1
      break;

    case 3:   // 74HC138D Y3(LOW_EN1) 동작
      digitalWrite(16, HIGH); // 74HC138D_A0
      digitalWrite(14, HIGH); // 74HC138D_A1      
      break;

    default: break;
  }
}

void IC_74HC238D_EN (byte selOut)
{
  switch(selOut)  // 입력 받은 값으로 74HC238D Yx 출력 선택
  {
    case 0:   // 74HC238D Y0 동작
      digitalWrite(12, LOW);  // 74HC238D_A0
      digitalWrite(13, LOW);  // 74HC238D_A1
      break;

    case 1:   // 74HC238D Y1(HIGH_EN3) 동작
      digitalWrite(12, HIGH); // 74HC238D_A0
      digitalWrite(13, LOW);  // 74HC238D_A1
      break;

    case 2:   // 74HC238D Y2(HIGH_EN2) 동작
      digitalWrite(12, LOW);  // 74HC238D_A0
      digitalWrite(13, HIGH); // 74HC238D_A1
      break;

    case 3:   // 74HC238D Y3(HIGH_EN1) 동작
      digitalWrite(12, HIGH); // 74HC238D_A0
      digitalWrite(13, HIGH); // 74HC238D_A1
      break;

    default: break;
  }
}
/* ---------------------------------------------------------------------------------------------------------- */

/* check input switches ------------------------------------------------------------------------------------- */
void prepare_sw (byte inStart, byte enNo, byte arrNo)
{
  if(inStart)
    SET_inputMode();  // pins 입력으로 변경

  // 밸브 입력 받기
  IC_74HC138D_EN(enNo);

  // wait transition time. max 225ns in datasheet
  previousms[arrNo] = millis();
  ++inputStep;
}

void chattering_sw5 (byte arrNo, byte *n1, byte *n2, byte *n3, byte *n4, byte *n5, u32_t setTime)
{
  // valve1 ~ valve5, valve6 ~ valve10까지 swtich 입력 5개씩 읽어오기
  currentms[arrNo] = millis();
  if(currentms[arrNo] - previousms[arrNo] >= setTime) // 1ms가 지나면
  {
    // IO15 = VALVE5_SW, high Active, LOW = 동작x
    if(digitalRead(15)) *n1 = 1;  // 안눌림
    else                *n1 = 0;  // 눌림

    // IO02 = VALVE4_SW, high Active, LOW = 동작x
    if(digitalRead(02)) *n2 = 1;
    else                *n2 = 0;

    // IO00 = VALVE3_SW, high Active, LOW = 동작x
    if(digitalRead(00)) *n3 = 1;
    else                *n3 = 0;

    // IO04 = VALVE2_SW, high Active, LOW = 동작x
    if(digitalRead(04)) *n4 = 1;
    else                *n4 = 0;

    // IO05 = VALVE1_SW, high Active, LOW = 동작x
    if(digitalRead(05)) *n5 = 1;
    else                *n5 = 0;

    // 74HC138D 회로와 상관 없는 상태로 변경
    IC_74HC138D_EN(0);

    previousms[arrNo] = currentms[arrNo] = 0;
    ++inputStep;
  }
}

void chattering_sw1 (byte arrNo, byte *n1, u32_t setTime)
{
  // pump switch 입력 1개 읽어오기
  currentms[arrNo] = millis();
  if(currentms[arrNo] - previousms[arrNo] >= setTime) // 1ms가 지나면
  {
    // IO05 = VALVE1_SW, high Active, LOW = 동작x
    if(digitalRead(05)) *n1 = 1;
    else                *n1 = 0;

    // 74HC138D 회로와 상관 없는 상태로 변경
    IC_74HC138D_EN(0);

    previousms[arrNo] = currentms[arrNo] = 0;
    ++inputStep;
  }
}

void check_sw1 (void)
{
  if(chatSW1 == chatSW6)  switchIN[5] = chatSW1;
  if(chatSW2 == chatSW7)  switchIN[4] = chatSW2;
  if(chatSW3 == chatSW8)  switchIN[3] = chatSW3;
  if(chatSW4 == chatSW9)  switchIN[2] = chatSW4;
  if(chatSW5 == chatSW10) switchIN[1] = chatSW5;

  chatSW1 = chatSW2 = chatSW3 = chatSW4 = chatSW5 = 1;
  chatSW6 = chatSW7 = chatSW8 = chatSW9 = chatSW10 = 0;

  ++inputStep;
}

void check_sw2 (void)
{
  if(chatSW11 == chatSW16)  switchIN[10] = chatSW11;
  if(chatSW12 == chatSW17)  switchIN[9] = chatSW12;
  if(chatSW13 == chatSW18)  switchIN[8] = chatSW13;
  if(chatSW14 == chatSW19)  switchIN[7] = chatSW14;
  if(chatSW15 == chatSW20)  switchIN[6] = chatSW15;

  chatSW11 = chatSW12 = chatSW13 = chatSW14 = chatSW15 = 1;
  chatSW16 = chatSW17 = chatSW18 = chatSW19 = chatSW20 = 0;

  ++inputStep;
}

void check_sw3 (void)
{
  if(chatSW21 == chatSW26)  switchIN[11] = chatSW21;

  chatSW21 = 1;
  chatSW26 = 0;

  ++inputStep;
}

void compare_all_switch_IN (void)
{
  for(byte i=1; i<SELECT_NUM; ++i)
  {
    if(switchIN[i] != sBackupIN[i]) // 이전 값이랑 비교하여
    {
      sBackupIN[i] = switchIN[i]; // 값이 변경 되었으면 이전 값으로 저장
      Serial.print("i = ");
      Serial.print(i);
      Serial.print(", switchIN[i] = ");
      Serial.println(switchIN[i]);
      send_valve_topic(i, switchIN[i]);
    }
  }
}

void check_input_switch (void)
{
  switch (inputStep)
  {
    case 1: // 한번 동작하고 딜레이 상태가 되면 다음 스텝으로 이동
      prepare_sw(0, 3, 1);  // 74HC138D Y3 ENABLE, 1번 배열 FLAG 설정;
      break;

    case 2: // 채터링 확인용 변수에 저장 후 다음 스텝으로 이동
      chattering_sw5(1, &chatSW1, &chatSW2, &chatSW3, &chatSW4, &chatSW5, 1);
      break;

    case 3: // 한번 동작하고 딜레이 상태가 되면 다음 스텝으로 이동
      prepare_sw(0, 2, 2);  // 74HC138D Y2 ENABLE, 2번 배열 FLAG 설정
      break;

    case 4: // 채터링 확인용 변수에 저장 후 다음 스텝으로 이동
      chattering_sw5(2, &chatSW11, &chatSW12, &chatSW13, &chatSW14, &chatSW15, 1);
      break;

    case 5: // 한번 동작하고 딜레이 상태가 되면 다음 스텝으로 이동
      prepare_sw(0, 1, 3);  // 74HC138D Y1 ENABLE, 3번 배열 FLAG 설정
      break;

    case 6: // 채터링 확인용 변수에 저장 후 다음 스텝으로 이동
      chattering_sw1(3, &chatSW21, 1);
      break;

    case 7: // 한번 동작하고 딜레이 상태가 되면 다음 스텝으로 이동
      prepare_sw(0, 3, 1);  // 74HC138D Y3 ENABLE, 1번 배열 FLAG 설정
      break;

    case 8: // 채터링 확인용 변수에 저장 후 다음 스텝으로 이동
      chattering_sw5(1, &chatSW6, &chatSW7, &chatSW8, &chatSW9, &chatSW10, 1);
      break;

    case 9: // 한번 동작하고 딜레이 상태가 되면 다음 스텝으로 이동
      prepare_sw(0, 2, 2);  // 74HC138D Y2 ENABLE, 2번 배열 FLAG 설정
      break;

    case 10:  // 채터링 확인용 변수에 저장 후 다음 스텝으로 이동
      chattering_sw5(2, &chatSW16, &chatSW17, &chatSW18, &chatSW19, &chatSW20, 1);
      break;

    case 11:  // 한번 동작하고 딜레이 상태가 되면 다음 스텝으로 이동
      prepare_sw(0, 1, 3);  // 74HC138D Y1 ENABLE, 3번 배열 FLAG 설정
      break;

    case 12:  // 채터링 확인용 변수에 저장 후 다음 스텝으로 이동
      chattering_sw1(3, &chatSW26, 1);
      break;

    case 13:
      check_sw1();
      break;

    case 14:
      check_sw2();
      break;

    case 15:
      check_sw3();
      break;

    case 16:
      compare_all_switch_IN();
      inputStep = 1;
      break;

    default:
      break;
  }
}
/* ---------------------------------------------------------------------------------------------------------- */

/* mqtt esp8266 --------------------------------------------------------------------------------------------- */
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void check_serial_topic (void)
{
  switch(selectTopic)
  {
    case 0:     client.subscribe(DownstreamTopic);      ++selectTopic;      break;
    case 1:     client.subscribe(DownstreamTopic100);   ++selectTopic;      break;
    case 2:     client.subscribe(DownstreamTopic11);    ++selectTopic;      break;
    case 3:     client.subscribe(DownstreamTopic12);    ++selectTopic;      break;
    case 4:     client.subscribe(DownstreamTopic13);    ++selectTopic;      break;
    case 5:     client.subscribe(DownstreamTopic14);    ++selectTopic;      break;
    case 6:     client.subscribe(DownstreamTopic15);    ++selectTopic;      break;
    case 7:     client.subscribe(DownstreamTopic16);    ++selectTopic;      break;
    case 8:     client.subscribe(DownstreamTopic17);    ++selectTopic;      break;
    case 9:     client.subscribe(DownstreamTopic18);    selectTopic = 0;    break;
    // case 0:     client.subscribe(DownstreamTopic);      ++selectTopic;      break;
    // case 1:     client.subscribe(DownstreamTopic1);     ++selectTopic;      break;
    // case 2:     client.subscribe(DownstreamTopic2);     ++selectTopic;      break;
    // case 3:     client.subscribe(DownstreamTopic3);     ++selectTopic;      break;
    // case 4:     client.subscribe(DownstreamTopic11);    ++selectTopic;      break;
    // case 5:     client.subscribe(DownstreamTopic12);    ++selectTopic;      break;
    // case 6:     client.subscribe(DownstreamTopic13);    ++selectTopic;      break;
    // case 7:     client.subscribe(DownstreamTopic14);    ++selectTopic;      break;
    // case 8:     client.subscribe(DownstreamTopic15);    ++selectTopic;      break;
    // case 9:     client.subscribe(DownstreamTopic16);    ++selectTopic;      break;
    // case 10:    client.subscribe(DownstreamTopic17);    ++selectTopic;      break;
    // case 11:    client.subscribe(DownstreamTopic18);    selectTopic = 0;    break;
    default:    break;
  }
}

void send_valve_topic (byte btnNum, byte OnOff)
{
  switch(btnNum)
  {
    case 11:
      if(OnOff) client.publish(UpstreamTopic100, "SW1,1");  // valve on
      else    client.publish(UpstreamTopic100, "SW1,0");  // valve off
      // if(OnOff)  client.publish(UpstreamTopic1, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}"); // valve on
      // else   client.publish(UpstreamTopic1, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}"); // valve off
      txValveState[1] = OnOff;
      break;

    case 1:
      if(OnOff) client.publish(UpstreamTopic100, "SW2,1");  // valve on
      else    client.publish(UpstreamTopic100, "SW2,0");  // valve off
      // if(OnOff)  client.publish(UpstreamTopic2, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}"); // valve on
      // else   client.publish(UpstreamTopic2, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}"); // valve off
      txValveState[2] = OnOff;
      break;

    case 6:
      if(OnOff) client.publish(UpstreamTopic100, "SW3,1");  // valve on
      else    client.publish(UpstreamTopic100, "SW3,0");  // valve off
      // if(OnOff)  client.publish(UpstreamTopic3, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}"); // valve on
      // else   client.publish(UpstreamTopic3, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}"); // valve off
      txValveState[3] = OnOff;
      break;

    case 2:
      if(OnOff) client.publish(UpstreamTopic11, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic11, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[11] = OnOff;
      break;

    case 3:
      if(OnOff) client.publish(UpstreamTopic12, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic12, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[12] = OnOff;
      break;

    case 4:
      if(OnOff) client.publish(UpstreamTopic13, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic13, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[13] = OnOff;
      break;

    case 5:
      if(OnOff) client.publish(UpstreamTopic14, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic14, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[14] = OnOff;
      break;

    case 7:
      if(OnOff) client.publish(UpstreamTopic15, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic15, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[15] = OnOff;
      break;

    case 8:
      if(OnOff) client.publish(UpstreamTopic16, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic16, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[16] = OnOff;
      break;

    case 9:
      if(OnOff) client.publish(UpstreamTopic17, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic17, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[17] = OnOff;
      break;

    case 10:
      if(OnOff) client.publish(UpstreamTopic18, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
      else    client.publish(UpstreamTopic18, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
      txValveState[18] = OnOff;
      break;

    default:
      break;
  }
}

void set_valve_response_flag (void)
{
  for(char i=1;i<4;++i)
  {
    if(txValveState[i] != rxValveState[i])  // 송신 밸브 값과, 수신 밸브 값이 다르면
    {
      if(!txFlag[i])          // flag가 설정되어 있지 않으면
      {
        txFlag[i] = 1;        // flag 설정 후 값 저장
        txTime[i] = millis();
      }
    }
    else
      txFlag[i] = 0;
  }

  for(char i=11;i<19;++i)
  {
    if(txValveState[i] != rxValveState[i])  // 송신 밸브 값과, 수신 밸브 값이 다르면
    {
      if(!txFlag[i])          // flag가 설정되어 있지 않으면
      {
        txFlag[i] = 1;        // flag 설정 후 값 저장
        txTime[i] = millis();
      }
    }
    else
      txFlag[i] = 0;
  }
}

void check_vlave_response (void)
{
  unsigned long currentMillis = millis();

  for(char i=1;i<4;++i)
  {
    if(txFlag[i])   // 송신과 수신 값이 달라서 flag가 설정되어 있고
    {
      if((currentMillis - txTime[i]) >= 5000 )  // 5초가 넘어가면
      {
        txTime[i] = currentMillis;

        switch(i)               // 재전송
        {
          case 1:
            if(txValveState[i]) client.publish(UpstreamTopic100, "SW1,1");  // valve on
            else        client.publish(UpstreamTopic100, "SW1,0");  // valve off
            // if(txValveState[i]) client.publish(UpstreamTopic1, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            // else       client.publish(UpstreamTopic1, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}"); // valve off
            break;

          case 2:
            if(txValveState[i]) client.publish(UpstreamTopic100, "SW2,1");  // valve on
            else        client.publish(UpstreamTopic100, "SW2,0");  // valve off
            // if(txValveState[i]) client.publish(UpstreamTopic2, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            // else       client.publish(UpstreamTopic2, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}"); // valve off
            break;        

          case 3:
            if(txValveState[i]) client.publish(UpstreamTopic100, "SW3,1");  // valve on
            else        client.publish(UpstreamTopic100, "SW3,0");  // valve off
            // if(txValveState[i]) client.publish(UpstreamTopic3, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            // else       client.publish(UpstreamTopic3, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}"); // valve off
            break;
        
          default:
            break;
        }
      }
    }
  }

  for(char i=11;i<19;++i)
  {
    if(txFlag[i])   // 송신과 수신 값이 달라서 flag가 설정되어 있고
    {
      if((currentMillis - txTime[i]) >= 25000 ) // 25초가 넘어가면
      {
        txTime[i] = currentMillis;

        Serial.print("25s, txValveState[] = ");
        for (char i = 0; i < 20; i++)
          Serial.print(txValveState[i]);
        Serial.print(" , rxValveState[] = ");
        for (char i = 0; i < 20; i++)
          Serial.print(rxValveState[i]);
        Serial.print(" , txFlag[] = ");
        for (char i = 0; i < 20; i++)
          Serial.print(txFlag[i]);
        Serial.println(" , txFlag[] = ");

        switch(i)               // 재전송
        {
          case 11:
            if(txValveState[i]) client.publish(UpstreamTopic11, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic11, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;

          case 12:
            if(txValveState[i]) client.publish(UpstreamTopic12, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic12, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;        

          case 13:
            if(txValveState[i]) client.publish(UpstreamTopic13, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic13, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;

          case 14:
            if(txValveState[i]) client.publish(UpstreamTopic14, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic14, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;

          case 15:
            if(txValveState[i]) client.publish(UpstreamTopic15, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic15, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;        

          case 16:
            if(txValveState[i]) client.publish(UpstreamTopic16, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic16, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;
        
          case 17:
            if(txValveState[i]) client.publish(UpstreamTopic17, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic17, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;

          case 18:
            if(txValveState[i]) client.publish(UpstreamTopic18, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");  // valve on
            else        client.publish(UpstreamTopic18, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");  // valve off
            break;

          default:
            break;
        }
      }
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  word dataStart;
    word EUIstart;
  byte base64rx[20];
    byte serialrx[20];
    byte selArr;
  char* convertRX;
  byte j = 0, k = 0;
    // Serial.print("Message arrived [");
    // Serial.print(topic);

    // Serial.print("] ");
    // for (int i = 0; i < length; i++) {
    // Serial.print((char)payload[i]);
    // }
    // Serial.println();

    if( ((char)payload[0] == 'V') && ((char)payload[1] == 'V') )
    {
        if( ((char)payload[2] == '1') && ((char)payload[4] == '1') )        rxValveState[1] = 1;
        else if( ((char)payload[2] == '1') && ((char)payload[4] == '0') )   rxValveState[1] = 0;
        else if( ((char)payload[2] == '2') && ((char)payload[4] == '1') )   rxValveState[2] = 1;
        else if( ((char)payload[2] == '2') && ((char)payload[4] == '0') )   rxValveState[2] = 0;
        else if( ((char)payload[2] == '3') && ((char)payload[4] == '1') )   rxValveState[3] = 1;
        else if( ((char)payload[2] == '3') && ((char)payload[4] == '0') )   rxValveState[3] = 0;
    }
    else
    {
    // 문자열 검색하여 시리얼 찾기
    for(word i=0;i<length;++i)
    {
      if((char)payload[i] == '[')
      {
        EUIstart = i;
        break;
      }
    }
    // 시리얼 저장하기
    Serial.print("devEUI = ");
    for(word i=EUIstart-27;i<EUIstart-11;++i)
    {
      serialrx[k] = payload[i];
      ++k;
    }
    Serial.print((char*)serialrx);
    Serial.print(", ");


    // 문자열 검색하여 시작 위치 찾기
    for(word i=length;i>0;--i)
    {
      if((char)payload[i] == ':')
      {
        dataStart = i;
        // Serial.print("lengthS");
        // Serial.print((char)payload[i]);
        // Serial.print(i);
        break;
      }
    }

    // base64에서 디코딩해야 하는 문자열 저장하기
    Serial.print("base64 data = ");
    for(word i=dataStart+2;i<dataStart+17;++i)
    {
      base64rx[j] = payload[i];
      ++j;
    }
    Serial.print((char*)base64rx);
    Serial.print(", ");

    // base64 디코딩
    rbase64.decode((const char*)base64rx);
    // Serial.println("\nConverted the String from Base64 : ");
    convertRX = rbase64.result();
    Serial.println(convertRX);

    // device EUI와 문자열 비교하여 변수로 처리
    if(!strncmp((const char*)serialrx, devEUI, 16))
      selArr = 0;
    // else if(!strncmp((const char*)serialrx, devEUI1, 16))
    //     selArr = 1;
    // else if(!strncmp((const char*)serialrx, devEUI2, 16))
    //     selArr = 2;
    // else if(!strncmp((const char*)serialrx, devEUI3, 16))
    //     selArr = 3;
    else if(!strncmp((const char*)serialrx, devEUI11, 16))
      selArr = 11;
    else if(!strncmp((const char*)serialrx, devEUI12, 16))
      selArr = 12;
    else if(!strncmp((const char*)serialrx, devEUI13, 16))
      selArr = 13;
    else if(!strncmp((const char*)serialrx, devEUI14, 16))
      selArr = 14;
    else if(!strncmp((const char*)serialrx, devEUI15, 16))
      selArr = 15;
    else if(!strncmp((const char*)serialrx, devEUI16, 16))
      selArr = 16;
    else if(!strncmp((const char*)serialrx, devEUI17, 16))
      selArr = 17;
    else if(!strncmp((const char*)serialrx, devEUI18, 16))
      selArr = 18;

    // 변환된 밸브 상태 값 저장
    if((char)convertRX[5] == '0') rxValveState[selArr] = 0;
    else if((char)convertRX[5] == '1') rxValveState[selArr] = 1;
  }


// {"applicationID":"3","applicationName":"valve-otaa","deviceName":"SK-LORA-GATE02","devEUI":"4b00ffeab9992068","rxInfo":[{"gatewayID":"60c5a8fffe74d37b","uplinkID":"c050d227-7872-4331-918c-ac3f40dbd8df","name":"SK-enc-rak7258-gw-01","rssi":-74,"loRaSNR":9.8,"location":{"latitude":0,"longitude":0,"altitude":0}}],"txInfo":{"frequency":904600000,"dr":4},"adr":false,"fCnt":19478,"fPort":1,"data":"QjM1NVYwRjAwMDA="}

    // Serial.print("selArr = ");
    // Serial.println(selArr);
    
  // if (rbase64.decode((const char*)base64rx) == RBASE64_STATUS_OK ) {
  //  Serial.println("\nConverted the String from Base64 : ");
  //  Serial.println(rbase64.result());
  //  Serial.println(" and ");
  //  convertRX = rbase64.result();
  //  Serial.println(convertRX);
  // }

//   if (rbase64.decode("SGVsbG8gVGhlcmUsIEkgYW0gZG9pbmcgR29vZC4=") == RBASE64_STATUS_OK ) {
//     Serial.println("\nConverted the String from Base64 : ");
//     Serial.println(rbase64.result());
//   }
}


void reconnect(byte boot) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    if(boot)
    {
      // Once connected, publish an announcement...
      snprintf ( msg, 150, "{\"ip\":\"%s\",\"rssi\":%ld,\"heap\":%ld}", WiFi.localIP().toString().c_str(), WiFi.RSSI(), ESP.getFreeHeap());
      client.publish(UpstreamTopic, msg);

      // booting = 0;
    }
      // ... and resubscribe
      client.subscribe(DownstreamTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* ---------------------------------------------------------------------------------------------------------- */
void setup() {
    pins_setup();
    // execute_relay_on();

    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}
char valveBoardTest = 0;  // valve on/off 확인용 설정 사용 여부
char stepSerial = 0;
void loop() {

  if(!valveBoardTest)
    check_input_switch();   // 스위치 입력 체크

  if (!client.connected()) {
    reconnect(1);
  }
  client.loop();

  if(!valveBoardTest)
    check_serial_topic();

  if(!valveBoardTest)
    set_valve_response_flag();    // 데이터 회신하여 값이 바뀌었는지 확인
  if(!valveBoardTest)
    check_vlave_response();     // 값이 안바뀌었으면 다시 전송

    if(Serial.available())
        stepSerial = Serial.read();

    switch(stepSerial)
    {
        case '1': // valve ON
            client.publish(UpstreamTopic, "{\"fPort\":1,\"data\":\"VkFMVkUx\",\"confirmed\":true}");
      Serial.println(stepSerial);
            stepSerial = 0;
            break;

        case '2': // valve OFF
            client.publish(UpstreamTopic, "{\"fPort\":1,\"data\":\"VkFMVkUw\",\"confirmed\":true}");
      Serial.println(stepSerial);
            stepSerial = 0;
            break;

        default: break;
    }
}

// void setup()
// {
  
// }

// void loop()
// {
  
// }

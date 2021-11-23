
// LoRa max payload = 242bytes
// max schedule = 18
// protocol : 6byte (= msg_type, size, id, commnad, schedule number, schedule length)
//        + 13byte (= schedule commnad, schedule start time, schdule processing time)
// 242 - 6 - (13 * 18.15385)
//  

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <ThreeWire.h>  // +library (Rtc by Makuna)
#include <RtcDS1302.h>  // +library (Rtc by Makuna)
#include "Adafruit_Si7021.h"    // +library Adafruit Si7021 Library
#include <Wire.h>

#define DEFAULT_SERIAL  1
#define CHECK_SEQUENCE  0
#define CHECK_VALUE   0


// pins define
#define VALVE_IN1   GPIO9
#define VALVE_IN2   GPIO8
#define MODE_3V3    GPIO10
#define FLOW_PWR_ON   GPIO11
#define FLOW_IN     GPIO12
#define PWR_5VDC_EN   GPIO14
#define ADC_BATTERY   ADC2
#define ADC_PRESSURE  ADC3
#define DS1302_CLK    GPIO3
#define DS1302_IO   GPIO2
#define DS1302_CE   GPIO1
#define VALVE_PWR_SW  GPIO6
#define VALVE_PWR_ON  GPIO5
#define LATCH_LE      GPIO7
#define LATCH_D       GPIO4

enum message_type
{
    CODE,
    UPDATE_DEVICE_REQUEST,
    UPDATE_DEVICE_RESPONSE, 
    REPORT_DEVICE_REQUEST,
    REPORT_DEVICE_RESPONSE,
    PING_REQUEST,
    PING_RESPONSE
};

enum command_type
{
    TYPE,
    VOPEN,
    VCLOSE,
    SCHEDULE,
    PAUSE_SCHEDULE,
    RESUME_SCHEDULE,
    CANCEL_SCHEDULE,
    OFF,
    REPORT_STATUS,
    MOPEN,
    MCLOSE
};


union long_long_type {
    char bytes[8];
    long long long_long_val;
};

union long_type {
    char bytes[4];
    long long_val;
};

union ulong_type {
    char bytes[4];
    unsigned long ulong_val;
}FL;

union float_type {
    char bytes[4];
    float float_val;
}BT, WP, TP, HM;


uint8_t message_code;
uint8_t message_size;
uint8_t response_id;
uint8_t command_byte;

boolean sendReport = false;
int resetCount = 0;


/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
//tailand-hydrofresh-sos-01
//uint8_t devEui[] = { 0x6c, 0x7b, 0xf1, 0xd3, 0x83, 0xef, 0xf6, 0x24 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x80, 0xe0, 0x50, 0x31, 0xa8, 0xa6, 0x72, 0xae, 0x0c, 0x7c, 0xf0, 0x19, 0x7e, 0x1e, 0xa6, 0x52 };

//tailand-hydrofresh-sos-test
//uint8_t devEui[] = { 0x8c, 0x82, 0x0c, 0xd2, 0xec, 0x49, 0x0c, 0x52 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x8f, 0xf0, 0xa0, 0x5f, 0x09, 0xd1, 0x02, 0x6a, 0x45, 0x29, 0x5b, 0xd7, 0x66, 0x63, 0x37, 0xee };

//tailand-hydrofresh-sos-02
//uint8_t devEui[] = { 0x24, 0x9c, 0x2e, 0x7e, 0xd9, 0x32, 0xd3, 0xd1 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x04, 0x97, 0x08, 0x09, 0xe6, 0xc0, 0x0f, 0x19, 0xa9, 0xea, 0xb5, 0xcb, 0x63, 0xc2, 0x2b, 0x47 };

//tailand-hydrofresh-sos-03
//uint8_t devEui[] = { 0xee, 0x77, 0x93, 0xba, 0xd4, 0xd3, 0xe3, 0xa9 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x55, 0xb5, 0x40, 0x05, 0x9c, 0x7e, 0xcb, 0xd5, 0x19, 0x4a, 0xdf, 0x60, 0xa4, 0x4d, 0xb5, 0x70 };

//tailand-hydrofresh-sos-04
//uint8_t devEui[] = { 0x2a, 0x78, 0x42, 0x4c, 0xa2, 0x98, 0xe9, 0x44 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x19, 0x12, 0x49, 0xfc, 0x9e, 0x52, 0x2d, 0x63, 0x86, 0xf8, 0x8a, 0x91, 0xf3, 0xed, 0x72, 0xc0 };

//sk-luonecity-21-03-valve-01
//uint8_t devEui[] = { 0x4a, 0x81, 0x94, 0x9c, 0x19, 0x66, 0xf7, 0x18 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x9c, 0x6a, 0x90, 0xa4, 0x7f, 0x94, 0x3e, 0xba, 0x7b, 0x06, 0x38, 0x3a, 0x79, 0x8a, 0x0e, 0xd8 };

//sk-luonecity-21-03-sos-valve-02
uint8_t devEui[] = { 0x7a, 0xeb, 0xe6, 0x0b, 0x9c, 0xd5, 0xc1, 0x6d };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x13, 0xce, 0x93, 0x0d, 0xc1, 0x1d, 0xad, 0x79, 0x42, 0x11, 0x54, 0x82, 0x16, 0xbd, 0x9b, 0xa4 };


/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

// LoRaWAN example Sub-Band usage. change the channel to 8-15
uint16_t userChannelsMask[6]={ 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 };
/*LoraWan channelsmask, default channels 0-7*/ 
//uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 10000;  // 10s
// uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

Adafruit_Si7021 sensor = Adafruit_Si7021();


// user function --------------------------------------------------------------
void pins_setup ()
{
  pinMode(MODE_3V3, OUTPUT);
  digitalWrite(MODE_3V3, HIGH);      // HIGH : boost mode, LOW : bypass mode

  /* VALVE */
  // drv8871 control pin Configuration (sleep mode = IN1 and IN2 low)
  pinMode(VALVE_IN1, OUTPUT);     // IN1 0, IN2 0 = sleep entered after 1 ms
  digitalWrite(VALVE_IN1, LOW);   // IN1 0, IN2 1 = Reverse (Current OUT2 → OUT1)
  pinMode(VALVE_IN2, OUTPUT);     // IN1 1, IN2 0 = Forward (Current OUT1 → OUT2)
  digitalWrite(VALVE_IN2, LOW);   // IN1 1, IN2 1 = Brake; low-side slow decay
  //  valve power enable pin Configuration
  pinMode(VALVE_PWR_SW, OUTPUT);
  digitalWrite(VALVE_PWR_SW, LOW);      // HIGH: enabled, LOW: disabled
  pinMode(VALVE_PWR_ON, OUTPUT);
  digitalWrite(VALVE_PWR_ON, LOW);      // HIGH: enabled, LOW: disabled

  /* FLOW */
  pinMode(FLOW_PWR_ON, OUTPUT);
  digitalWrite(FLOW_PWR_ON, LOW);      // Active high
  pinMode(FLOW_IN, INPUT);

  // MOTOR
  pinMode(LATCH_D,OUTPUT);
    pinMode(LATCH_LE,OUTPUT);

  // PRESSURE
  pinMode(PWR_5VDC_EN, OUTPUT);
  digitalWrite(PWR_5VDC_EN, LOW);

    // // Real-Time Clock pin Configuration
    pinMode(DS1302_CE, OUTPUT);
    pinMode(DS1302_IO, OUTPUT);
    pinMode(DS1302_CLK, OUTPUT);
}


/* LoRa ---------------------------------------------------------------------------------------------------------------------- */
uint8_t check_com_count = 0;

void count_LoRa_join (void)
{
  if(IsLoRaMacNetworkJoined)  // join이면
    check_com_count = 0;
  else
    ++ check_com_count;
}
/* --------------------------------------------------------------------------------------------------------------------------- */


/* DRV8871 Control ----------------------------------------------------------------------------------------------------------- */
#define TURN_ON_TIME    10      // ms                   // IC가 동작하도록 기다리는 시간
#define DELAY_TIME      75      // ms                   // valve 동작하는 시간

uint8_t valveCommand = 0; 
uint8_t valveState = 0;
uint8_t valveStep = 0;
unsigned long checkVtime = 0x00000000;

void start_Valve_step (void)
{
  if(valveState != valveCommand)      // 밸브가 이전 상태와 다르게 동작하라는 명령이 오면
  {
    if(!valveStep)     // 현재 밸브가 동작 중이 아니면
      valveStep = 1;                  // 밸브 동작 시작
  }
}

void Start_Valve_OFF (void)
{
    valveCommand = 0;

    if(valveState)                  // 밸브가 켜져있으면
        valveStep = 1;              // 밸브 동작
}

void Init_Valve (void)
{
    digitalWrite(VALVE_IN1, LOW);     // Coast;
    digitalWrite(VALVE_IN2, LOW);
}

void Valve_Operation (byte sel)
{
    if(sel)
    {
        digitalWrite(VALVE_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
        digitalWrite(VALVE_IN2, HIGH);
    }
    else
    {
        digitalWrite(VALVE_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
        digitalWrite(VALVE_IN2, LOW);
    }
}

void control_Valve (unsigned long waitTime)
{
    switch(valveStep)
    {
        case 1:
            digitalWrite(VALVE_PWR_SW, HIGH);    // ENABLE(ON) Active High
            digitalWrite(VALVE_PWR_ON, HIGH);    // Pull high to enable the device, pull low to disable the device.
            checkVtime = millis();
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("vS=");
            Serial.println(valveStep);
        #endif
            break;

        case 2:
            if(waitTime - checkVtime >= TURN_ON_TIME)      // Wait Turn On Time
                ++ valveStep;
            break;

        case 3:
      Valve_Operation(valveCommand);
            checkVtime = millis();
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("vS=");
            Serial.println(valveStep);
        #endif
            break;

        case 4:
            if(waitTime - checkVtime >= DELAY_TIME)     // Operating pulse width
                ++ valveStep;
            break;

        case 5:
            digitalWrite(VALVE_PWR_SW, LOW);
            digitalWrite(VALVE_PWR_ON, LOW);
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("vS=");
            Serial.println(valve2step);
        #endif
            break;

        case 6:
      Init_Valve();
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("v1s=");
            Serial.println(valve2step);
        #endif
            break;

        case 7:
            valveState = valveCommand;
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("v1s=");
            Serial.print(valve1step);
            Serial.print(", v2s=");
            Serial.println(valve2step);
        #endif
            break;

        case 8:
            valveStep = 0;
        #if DEBUG_MODE
            Serial.print("v1s=");
            Serial.print(valve1step);
            Serial.print(", v2s=");
            Serial.println(valve2step);
        #endif
            break;
        default: break;
    }
}

uint8_t valveOFF = 0;
void Check_Valve (void)             // 밸브가 켜져 있으면 문제가 되므로 일정 시간 이후에 꺼지도록하는 함수
{
// check list - 10 변경하여 통신 끊김 빨리 재현시켜 밸브가 꺼지는지 확인
// 1. 밸브가 켜져 있고, 통신이 끊기면 꺼지는지 확인
// 2. 통신이 끊기고 다시 접속하면 정상 동작 (ON, OFF) 확인
    if( (check_com_count > 10) && (!valveOFF) && (valveState) && (!valveStep) )    // 30번 전송하는 동안 통신이 끊기면 (10s * 10 = 100s)
    {
        valveOFF = 1;
        Start_Valve_OFF();

        while (valveStep)  // confirm을 10초씩 대기하는 경우가 생기므로
        {                                   // 다른 함수 진입하지 않고 while에서 밸브 꺼지도록 처리
            unsigned long countMillis = millis();
            control_Valve(countMillis);
        }
    }
}
/* --------------------------------------------------------------------------------------------------------------------------- */


/* Flowmeter ----------------------------------------------------------------------------------------------------------------- */
#define FLOW_TIME       1000    // 1000ms = 1s          // 유량 측정 하는 시간

uint8_t flowStep = 0;
unsigned long checkFtime = 0x00000000;

void getFlow (void)
{
    FL.ulong_val ++;    //  low가 인식, high가 미인식
}

void flow_sequence (unsigned long readTime)
{
    switch(flowStep)
    {
        case 1:
//            FL.ulong_val = 0;
            digitalWrite(FLOW_PWR_ON, HIGH); // Flow Sensor power ON
            Wire.begin();
      attachInterrupt(FLOW_IN,getFlow,FALLING); // example
            // attachInterrupt(digitalPinToInterrupt(FLOW_IN), getFlow, FALLING);    // Flow Sensor interrupt ON
            checkFtime = millis();    // 현재 시간 저장
            ++ flowStep;            // 다음 단계로 이동

        #if DEBUG_MODE
            Serial.println("f1");
        #endif
            break;

        case 2:
            if(readTime - checkFtime >= FLOW_TIME)    // 설정 시간만큼 유량값 측정
                ++ flowStep;
            break;

        case 3:
            detachInterrupt(FLOW_IN);        // 유량 인터럽트 정지
            Wire.end();
            digitalWrite(FLOW_PWR_ON, LOW); // Flow power OFF
            flowStep = 0;
        #if DEBUG_MODE
            Serial.println("m3");
        #endif
        #if CHECK_VALUE
            Serial.print("flow = ");
            Serial.println(FL.ulong_val);
        #endif
            break;

        default: break;
    }
}

void start_flow_step (void)
{
  flowStep = 1;
}
/* --------------------------------------------------------------------------------------------------------------------------- */


/* Read ADC Voltage ---------------------------------------------------------------------------------------------------------- */
void check_battery_voltage (void)
{
  char i;
  word adcBT[6] = { 0, 0, 0, 0, 0, 0 };
  uint32_t sumBT = 0;
  float aveBT = 0;

  for(i=0; i<6; ++i)
  {
    adcBT[i] = analogRead(ADC_BATTERY);    // 배터리 전압 ADC로 읽어오기 (mV)
    delay(1);               // 1ms 대기
  }

  for(i=1; i<6; ++i)
    {
        sumBT = sumBT + (uint32_t)adcBT[i];   // 5번 읽은 전압 합
    #if CHECK_VALUE
        Serial.print("adc = ");
        Serial.print(adcBT[i]);
        Serial.print(", sum = ");
        Serial.println(sumBT);
    #endif
    }
    
  aveBT = (float)sumBT / 5;             // 평균내기

#if DEBUG_MODE
    Serial.print("aveBT = ");
    Serial.println(adcBT);
#endif

    // 전압 분배로 인하여 약 0.62921배 낮아진 값을 ADC로 읽어옴
    // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
    // 읽어온 ADC 전압 값을 실제 전압으로 변환
    BT.float_val = aveBT / 0.62921;     // mV

#if CHECK_VALUE
    Serial.print("BTLV = ");
    Serial.println(BT.float_val, 2);
#endif

    // HM4100B 기준 0~4bar
#if DEBUG_MODE
    Serial.print("btLV = ");
    Serial.println(batteryLevel);
#endif
}

void check_pressure_voltage (void)
{
  char i;
  word adcWP[6] = { 0, 0, 0, 0, 0, 0 };
  uint32_t sumWP = 0;
  float aveWP = 0;

    digitalWrite(PWR_5VDC_EN, HIGH);        // pressure voltage ON;
    Wire.begin();
    delay(2);                               // waiting pressure turn on

  for(i=0; i<6; ++i)
  {
    adcWP[i] = analogRead(ADC_PRESSURE);    // 배터리 전압 ADC로 읽어오기
    delay(1);               // 1ms 대기
  }

  for(i=1; i<6; ++i)
    sumWP = sumWP + (uint32_t)adcWP[i];   // 5번 읽은 전압 합
  aveWP = (float)sumWP / 5;             // 평균내기

#if DEBUG_MODE
    Serial.print("aveBT = ");
    Serial.println(adcBT);
#endif

    // 전압 분배로 인하여 약 0.47663배 낮아진 값을 ADC로 읽어옴
    // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
    // 읽어온 ADC 전압 값을 실제 전압으로 변환
  WP.float_val = aveWP / 0.47663;     // mV
  Wire.end();

    digitalWrite(PWR_5VDC_EN, LOW);        // pressure voltage OFF;

#if CHECK_VALUE
    Serial.print("WPLV = ");
    Serial.println(WP.float_val, 2);
#endif
    // 압력계 정해지면 압력계 값으로 변경하기
#if DEBUG_MODE
    Serial.print("WPLV = ");
    Serial.println(pressureLevel);
#endif
}

uint8_t adcStep = 0;
void adc_sequence (unsigned long readTime)
{
    switch(adcStep)
  {
    case 1:
            check_battery_voltage();        // 배터리 전압 읽어서 저장
      ++ adcStep;
      break;

    case 2:
      check_pressure_voltage();       // 압력계 전압 읽어서 저장
      adcStep = 0;
      break;

    default: break;
  }
}

void start_adc_step (void)
{
  adcStep = 1;
}
/* --------------------------------------------------------------------------------------------------------------------------- */


/* control Motor ------------------------------------------------------------------------------------------------------------- */
uint8_t motorCommand = 0;
uint8_t motorState = 0;
uint8_t motorStep = 0;
unsigned long checkLtime = 0x00000000;

void start_Motor_step (void)
{
  if(motorState != motorCommand)      // 밸브가 이전 상태와 다르게 동작하라는 명령이 오면
  {
    if(!motorStep)            // 현재 모터가 동작중이 아니면
      motorStep = 1;                  // 모터 동작 시작
  }
}

void control_Motor (unsigned long readTime)
{
  switch(motorStep)
  {
    case 1:
      if(motorCommand)
                digitalWrite(LATCH_D,LOW);  // on
      else
                digitalWrite(LATCH_D,HIGH); // off
      ++ motorStep;

        #if CHECK_SEQUENCE
            Serial.println("m1");
        #endif
      break;

        case 2:
            digitalWrite(LATCH_LE,HIGH);  // Turn on MAX 19ns
            checkLtime = millis();
            ++ motorStep;

        #if CHECK_SEQUENCE
            Serial.println("m1");
        #endif
            break;

        case 3:
            if((readTime - checkLtime) >= 1)    // 1ms가 지나면
                ++ motorStep;
            break;

        case 4:
            digitalWrite(LATCH_LE,LOW);     // Turn off
            motorState = motorCommand;
            ++ motorStep;

        #if CHECK_SEQUENCE
            Serial.println("m4");
        #endif
            break;

        case 5:
            motorStep = 0;
            break;

        default: break;
  }
}

void start_motor_step (void)
{
  motorStep = 1;
}

void Init_motor (void)
{
    digitalWrite(LATCH_D,HIGH);     // Data OUT off
    delay(1);
    digitalWrite(LATCH_LE,HIGH);  // Turn on MAX 19ns
    delay(1);
    digitalWrite(LATCH_LE,LOW);     // Turn off
}
/* --------------------------------------------------------------------------------------------------------------------------- */

/* Si7021-A20 I2C Humidity and Temperature Sensor ---------------------------------------------------------------------------- */
void I2C_setup (void)
{
    pinMode(Vext, OUTPUT);
}

void ht_sequence (void)
{
    // Vext ON
    digitalWrite(Vext, LOW);
    Wire.begin();
    delay(50);

    if (!sensor.begin())
    {
        Serial.println("Did not find Si7021 sensor!");
        Wire.end();
        digitalWrite(Vext, HIGH);
        delay(1000);
        return;
    }

    HM.float_val = sensor.readHumidity();
    TP.float_val = sensor.readTemperature();
    Wire.end();

#if CHECK_VALUE
    Serial.print("Humidity:    ");
    Serial.print(HM.float_val, 3);
    Serial.print("\tTemperature: ");
    Serial.println(TP.float_val, 3);
#endif

    // Vext OFF
    digitalWrite(Vext, HIGH);
}
/* --------------------------------------------------------------------------------------------------------------------------- */

uint8_t nodeWoke = 0;
uint8_t tx_buf = 0;
/* Prepares the payload of the frame */

static void prepareTxFrame( uint8_t port )
{
    switch(message_code)
    {
        case UPDATE_DEVICE_REQUEST:
        #if CHECK_SEQUENCE
            Serial.println("t1");
        #endif
            appPort = 2;
            appDataSize = 4;
            appData[0] = UPDATE_DEVICE_RESPONSE;
            appData[1] = 2;
            appData[2] = response_id;
            appData[3] = 0; // 정상
            // appData[3] = 1;  // 비정상, 비정상적인 경우에는 command와 이후 내용 보내기

            // 정상 수신 응답 후 동작
            if(tx_buf == REPORT_STATUS){
                message_code = REPORT_DEVICE_REQUEST;
                sendReport = true;
            }else
                message_code = 0;
            break;

        case REPORT_DEVICE_REQUEST:
        #if CHECK_SEQUENCE
            Serial.println("t2");
        #endif
            appDataSize = 42;
            appData[0] = REPORT_DEVICE_REQUEST;
            appData[1] = 40;
            appData[2] = response_id;
            appData[3] = 6; // Event Count = valve, battery, flow, pressure, temperature, humidity
            // valve
            if(valveState)  appData[4] = 1;   // event type : valve ON
            else      appData[4] = 2;   // event type : valve OFF
            appData[5] = 1;           // event Size
            appData[6] = 0;           // event status : 0 Success
            // battery
            appData[7] = 3;                 // event type : battery status
            appData[8] = 5;                 // event Size
            appData[9] = 2;                 // event status : 2 Floating value
            appData[10] = BT.bytes[3];      // value
            appData[11] = BT.bytes[2];      // value
            appData[12] = BT.bytes[1];      // value
            appData[13] = BT.bytes[0];      // value
            // sensor
            appData[14] = 4;                // event type : sensor value
            appData[15] = 5;                // event Size
            appData[16] = 3;                // event status : 3 Int value
            appData[17] = FL.bytes[3];      // value
            appData[18] = FL.bytes[2];      // value
            appData[19] = FL.bytes[1];      // value
            appData[20] = FL.bytes[0];      // value
            // pressure
            appData[21] = 6;                // event type : pressure
            appData[22] = 5;                // event Size
            appData[23] = 2;                // event status : 2 Floating value
            appData[24] = WP.bytes[3];      // value
            appData[25] = WP.bytes[2];      // value
            appData[26] = WP.bytes[1];      // value
            appData[27] = WP.bytes[0];      // value
            // temperature
            appData[28] = 7;                // event type : temperature
            appData[29] = 5;                // event Size
            appData[30] = 2;                // event status : 2 Floating value
            appData[31] = TP.bytes[3];      // value
            appData[32] = TP.bytes[2];      // value
            appData[33] = TP.bytes[1];      // value
            appData[34] = TP.bytes[0];      // value
            // humidity
            appData[35] = 8;                 // event type : humidity
            appData[36] = 5;                 // event Size
            appData[37] = 2;                 // event status : 2 Floating value
            appData[38] = HM.bytes[3];      // value
            appData[39] = HM.bytes[2];      // value
            appData[40] = HM.bytes[1];      // value
            appData[41] = HM.bytes[0];      // value

            tx_buf = 0;
            message_code = 0;
            break;

        case PING_REQUEST:    // ping 요청이나
        default:        // 기본은 PING_RESPONSE로 응답
        #if CHECK_SEQUENCE
            Serial.println("t3");
        #endif
            appDataSize = 5;
            appData[0] = PING_RESPONSE;
            appData[1] = message_size;
            appData[2] = response_id;
            if(valveState)  appData[4] = 1;   // event type : valve ON
            else      appData[4] = 2;   // event type : valve OFF
            break;
    }

    nodeWoke = 1;               // adc flow valve 제어 시작
}

//downlink data handle function example
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
#if DEFAULT_SERIAL
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
  {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
#endif
  uint32_t color=mcpsIndication->Buffer[0]<<16|mcpsIndication->Buffer[1]<<8|mcpsIndication->Buffer[2];
#if(LoraWan_RGB==1)
  turnOnRGB(color,5000);
  turnOffRGB();
#endif

  message_code = mcpsIndication->Buffer[0];
  message_size = mcpsIndication->Buffer[1];
  response_id = mcpsIndication->Buffer[2];

  if(message_code == UPDATE_DEVICE_REQUEST)
  {
    command_byte = mcpsIndication->Buffer[3];

    switch(command_byte)
    {
      case VOPEN:
        valveCommand = 1;
        start_Valve_step();
        break;

      case VCLOSE:
        valveCommand = 0;
        start_Valve_step();
        break;

      case SCHEDULE:
// 추후에 수신 및 이벤트 동작 구현
/*                    for (int i = 0; i < event_count; i++)
      {
        buffer[index++] = events[i].type; //event type
        buffer[index++] = get_event_chunk_size(&events[i]) - 2; //event size
        buffer[index++] = events[i].event_status;

        if((events[i].event_status == 2) ||
          (events[i].event_status == 3))
        {
          int j;
          for (j = 0 ; j < 4; j++)
          {
            buffer[index++] = events[i].value.bytes[j];
          }
        }

        if (events[i].is_time_exists)
        {
          int j;
          for (j = 0; j < 8; j++)
          {
            buffer[index++] = events[i].time.bytes[j];
          }
        }
      }*/
        break;

      case PAUSE_SCHEDULE:
        break;

      case RESUME_SCHEDULE:
        break;

      case CANCEL_SCHEDULE:
        break;

      case OFF:
        break;

      case REPORT_STATUS:
                tx_buf = REPORT_STATUS;
        break;

      case MOPEN:
        valveCommand = 1;
        start_Valve_step();
        motorCommand = 1;
        start_Motor_step();
        break;

      case MCLOSE:
        valveCommand = 0;
        start_Valve_step();
        motorCommand = 0;
        start_Motor_step();
        break;

      default: break;
    }
  }
  else if(message_code == REPORT_DEVICE_RESPONSE) // Application > Device
  {
  }
  else if(message_code == PING_REQUEST) // Application > Device
  {
  }
}

ThreeWire myWire(DS1302_IO,DS1302_CLK,DS1302_CE); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

void RTC_setup (void)
{
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
#if DEBUG_MODE
    printDateTime(compiled);
    Serial.println();
#endif

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

    #if DEBUG_MODE
        Serial.println("RTC lost confidence in the DateTime!");
    #endif
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
    #if DEBUG_MODE
        Serial.println("RTC was write protected, enabling writing now");
    #endif
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
    #if DEBUG_MODE
        Serial.println("RTC was not actively running, starting now");
    #endif
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
    #if DEBUG_MODE
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
    #endif
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
    #if DEBUG_MODE
        Serial.println("RTC is newer than compile time. (this is expected)");
    #endif
    }
    else if (now == compiled) 
    {
    #if DEBUG_MODE
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    #endif
    }
}



void setup() {
  boardInitMcu();
  Serial.begin(115200);
#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();

    pins_setup();
    I2C_setup();
    Init_motor();
}

void loop()
{
  uint32_t msecTime = millis();
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
        #if CHECK_SEQUENCE
            Serial.println("d1");
        #endif

#if(AT_SUPPORT)
      getDevParam();
#endif
      printDevParam();
      LoRaWAN.init(loraWanClass,loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
        #if CHECK_SEQUENCE
            Serial.println("d2");
        #endif

      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
            if(nodeWoke)
            {
                switch(nodeWoke)
                {
                    case 1:
                        start_adc_step();
                        start_flow_step();
                        // 현재 시간 읽기 추가
                        ++ nodeWoke;
                    #if CHECK_SEQUENCE
                        Serial.println("n1");
                    #endif
                        break;

                    case 2:
                        adc_sequence(msecTime);
                        flow_sequence(msecTime);
                        control_Valve(msecTime);
                        control_Motor(msecTime);

                        if( (!valveStep) && (!flowStep) && (!adcStep) && (!motorStep) )
                        {
                        #if CHECK_SEQUENCE
                            Serial.println("n2");
                        #endif
                            ++ nodeWoke;
                        }
                        break;

                    case 3:
                        nodeWoke = 0;
                    #if CHECK_SEQUENCE
                        Serial.println("n4");
                    #endif
                        break;

                    default:
                        break;
                }
            }
            else
            {
                count_LoRa_join();          //  join 확인
                ht_sequence();              // 온도, 습도 읽기 시작
                deviceState = DEVICE_STATE_CYCLE;
                prepareTxFrame( appPort );
                LoRaWAN.send();
                if(sendReport){
                  resetCount ++;
                  if(resetCount == 2){
                    deviceState = DEVICE_STATE_INIT;
                    message_code = REPORT_DEVICE_REQUEST;
                    sendReport = false;
                    resetCount = 0;
                  }
                }
            }
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
        #if CHECK_SEQUENCE
            Serial.println("d4");
        #endif
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
            LoRaWAN.sleep();
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }

  Check_Valve();
}


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
#define CHECK_SEQUENCE  1
#define CHECK_VALUE   1


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

char flowChar[5] = "0000";
char battChar[4] = "000";
char tempChar[4] = "000";
char humiChar[4] = "000";
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
uint8_t devEui[] = { 0x47, 0x97, 0xaa, 0xfe, 0x41, 0x8b, 0x68, 0x54 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xe9, 0x9e, 0xf9, 0x3b, 0x37, 0x71, 0x34, 0xf5, 0x93, 0x5e, 0x75, 0x91, 0x16, 0x47, 0x6d, 0x4f };


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

/* DRV8871 Control ----------------------------------------------------------------------------------------------------------- */
#define TURN_ON_TIME    10      // ms                   // IC가 동작하도록 기다리는 시간
#define DELAY_TIME      75      // ms                   // valve 동작하는 시간

uint8_t valveCommand = 0; 
uint8_t valveState = 0;
uint8_t valveStep = 0;
uint8_t valveStat;
unsigned long checkVtime = 0x00000000;
/* --------------------------------------------------------------------------------------------------------------------------- */

/* Flowmeter ----------------------------------------------------------------------------------------------------------------- */
#define FLOW_TIME       1000    // 1000ms = 1s          // 유량 측정 하는 시간

uint8_t flowStep = 0;
unsigned long checkFtime = 0x00000000;

/* --------------------------------------------------------------------------------------------------------------------------- */
/* Battery ----------------------------------------------------------------------------------------------------------------- */
uint8_t adcStep = 0;
/* --------------------------------------------------------------------------------------------------------------------------- */


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

  // PRESSURE
  pinMode(PWR_5VDC_EN, OUTPUT);
  digitalWrite(PWR_5VDC_EN, LOW);

    // // Real-Time Clock pin Configuration
    pinMode(DS1302_CE, OUTPUT);
    pinMode(DS1302_IO, OUTPUT);
    pinMode(DS1302_CLK, OUTPUT);
}


uint8_t nodeWoke = 0;
uint8_t tx_buf = 0;
/* Prepares the payload of the frame */

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
}

void loop()
{
  uint32_t msecTime = millis();
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
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

                        if( (!valveStep) && (!flowStep) && (!adcStep))
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
                resetCount ++;
                if(resetCount == 5){
                  deviceState = DEVICE_STATE_INIT;
                  resetCount = 0;
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

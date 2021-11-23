#include "LoRaWan_APP.h"
#include "Arduino.h"

#define VALVE_1_PIN     GPIO5
#define CHANNEL         1
#define DEVICE_NO       1

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */
#if DEVICE_NO == 1
  /* skec-dongsung-rak7201-lora-buzzer-1 */ 
  uint8_t devEui[] = { 0xd1, 0x73, 0xbf, 0xf5, 0x70, 0x8f, 0x4a, 0xcd };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x62, 0x8C, 0x99, 0x3C, 0xBE, 0xAE, 0xDD, 0x67, 0xCF, 0x0E, 0xED, 0x28, 0xB8, 0x74, 0x69, 0x8D };
#elif DEVICE_NO == 2
  /* skec-dongsung-rak7201-lora-buzzer-2 */ 
  uint8_t devEui[] = { 0xd3, 0xe4, 0x12, 0x28, 0x30, 0x23, 0x25, 0x32 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x5B, 0xF3, 0x79, 0x7A, 0xDC, 0x63, 0xC0, 0x5C, 0xA5, 0x39, 0x7A, 0xDC, 0xBD, 0x38, 0xC3, 0x2E };
#elif DEVICE_NO == 3
  /* skec-dongsung-rak7201-lora-buzzer-3 */ 
  uint8_t devEui[] = { 0xeb, 0x3f, 0x0f, 0xf9, 0xf8, 0x67, 0xd5, 0x45 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x99, 0xE4, 0x36, 0xC0, 0x19, 0x10, 0x3F, 0xD2, 0xBA, 0x19, 0x1F, 0x30, 0xAF, 0xA4, 0xBF, 0x15 };
#elif DEVICE_NO == 4
  /* skec-dongsung-rak7201-lora-buzzer-4 */ 
  uint8_t devEui[] = { 0x79, 0x3a, 0x27, 0x79, 0x7a, 0x74, 0xe4, 0x0e };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x7F, 0xF5, 0x0B, 0x0E, 0x39, 0x29, 0x08, 0x78, 0x6E, 0x66, 0x20, 0xEE, 0x2D, 0xCD, 0x8A, 0xEE };
#endif

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
uint32_t appTxDutyCycle = 15000;

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

/* Valve control */
uint8_t valveStat;
uint8_t valveCommand[CHANNEL] = {0,};

/* Battery */
char battChar[4] = "000";

union float_type {
    char bytes[4];
    float float_val;
}BT;


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

void setup() {
  boardInitMcu();
  Serial.begin(115200);
#if(AT_SUPPORT)
  enableAt();
#endif
  pinSetup();
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
  
}

void loop()
{
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
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
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
}

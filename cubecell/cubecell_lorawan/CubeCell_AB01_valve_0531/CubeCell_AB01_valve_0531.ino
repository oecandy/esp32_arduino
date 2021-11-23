#include "LoRaWan_APP.h"
#include "Arduino.h"

#define VALVE_OPEN_1_1 GPIO5
#define VALVE_OPEN_1_2 GPIO6
#define FLOW_IN        GPIO12
/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* test valve */
//uint8_t devEui[] = { 0x9a, 0x67, 0x26, 0x73, 0x55, 0x02, 0xb6, 0xea };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x0B, 0x09, 0x03, 0xA4, 0x55, 0xC5, 0x43, 0x1F, 0xE5, 0x04, 0x28, 0x7E, 0x39, 0x90, 0xF9, 0xC0 };

/* idle-valve-01*/ 
//uint8_t devEui[] = { 0x1d, 0x60, 0x56, 0xe2, 0x59, 0x94, 0x58, 0x05 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xDF, 0xEE, 0xE3, 0x9A, 0x41, 0x2D, 0x3F, 0xD8, 0xD2, 0x7B, 0x30, 0xD3, 0xC1, 0xAC, 0x49, 0x90 };

/* idle-valve-02*/ 
//uint8_t devEui[] = { 0x2a, 0xc0, 0xe3, 0x90, 0x0c, 0xee, 0xb4, 0x1c };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xDB, 0x2B, 0x7E, 0xB6, 0x1C, 0x2A, 0x86, 0xB9, 0xD3, 0x1D, 0xD3, 0xA5, 0xCF, 0x9B, 0x90, 0x77};

/* idle-valve-03*/ 
//uint8_t devEui[] = { 0x6e, 0x61, 0x9c, 0x6f, 0x91, 0x20, 0xae, 0xe6 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xA9, 0xCA, 0xE1, 0xFC, 0x56, 0x60, 0xF3, 0xE4, 0xD0, 0x51, 0x02, 0x1F, 0x27, 0x06, 0x36, 0x2E };

/* idle-valve-04*/
//uint8_t devEui[] = { 0x1f, 0x81, 0x4b, 0x0c, 0xc2, 0x27, 0x82, 0x66 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xF8, 0x27, 0xA0, 0xDB, 0x47, 0x23, 0x64, 0x87, 0x9B, 0x25, 0x0A, 0x4F, 0x0F, 0x82, 0x46, 0x30 };

/* idle-valve-05*/
//uint8_t devEui[] = { 0x40, 0x73, 0x49, 0xce, 0xcd, 0x25, 0x26, 0xb0 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x97, 0x5B, 0x69, 0x5A, 0x34, 0x55, 0x2D, 0x23, 0x2D, 0x4C, 0x75, 0xA2, 0x5E, 0x31, 0x69, 0x3D };

/* idle-valve-06*/
//uint8_t devEui[] = { 0xb8, 0x0c, 0xb0, 0x74, 0xde, 0x1a, 0x10, 0x9e };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xCE, 0x3A, 0x13, 0x5B, 0x64, 0x92, 0xA7, 0x0F, 0x91, 0x1C, 0x34, 0xE6, 0xCC, 0x7B, 0x4E, 0xB2 };

/* idle-valve-07*/ 
//uint8_t devEui[] = { 0x58, 0x4c, 0xa3, 0x24, 0x02, 0x25, 0x02, 0x1d };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xE5, 0x5D, 0xB6, 0x48, 0xCB, 0xCC, 0xD4, 0x53, 0x83, 0x63, 0x92, 0x86, 0xCF, 0x66, 0xFC, 0x39 };

/* NAJU LG CNS Valve 2*/ 
//uint8_t devEui[] = { 0xc8, 0x32, 0x10, 0x1e, 0xb6, 0x53, 0x32, 0xb6 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xA9, 0xDF, 0x66, 0xD5, 0xDB, 0x44, 0xA2, 0xA4, 0x7B, 0x4B, 0x9B, 0xC9, 0xBA, 0x6A, 0xF5, 0xE7 };

/* NAJU LG CNS Valve 3*/ 
uint8_t devEui[] = { 0x59, 0xc1, 0x19, 0xd2, 0x4a, 0x6b, 0x18, 0x11 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x2E, 0x21, 0x73, 0xD2, 0x4B, 0x68, 0x35, 0x97, 0xD2, 0x57, 0x28, 0x1C, 0xE3, 0x81, 0xE8, 0xB3 };

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
uint32_t appTxDutyCycle = 10000;

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
uint8_t valveCommand = 0;

/* Flowmeter */
char flowChar[5] = "0000";

uint8_t flowStep = 0;

union ulong_type {
    char bytes[4];
    unsigned long ulong_val;
}FL;

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
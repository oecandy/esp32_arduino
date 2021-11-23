#include "LoRaWan_APP.h"
#include "Arduino.h"

#include <ModbusMaster.h>
#include "softSerial.h"

// Variables abo\\\\\ut LoraDevice Setting, This is not LoraWan solution, 
// only using LoRaWan Library that why the names of variables contain 'LoRaWAN'.
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             20        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 250 // Define the payload size here

// MODBUS settings
#define MAX485_DE                                   GPIO5
#define MAX485_RE_NEG                               GPIO6
#define MAX485_RXD                                  GPIO2
#define MAX485_TXD                                  GPIO1
#define REGISTER_SIZE                               10 // MODBUS register size
#define SLAVE_ID                                    1  // gas sensor 485 slave id
#define INTERVAL_TIME                               10000

// device profiles
#define DEVICE_TYPE       10 // 10: fire alarm, 11: Buzzer
#define SENSOR_NO         3

#if   SENSOR_NO == 1
  #define   SERIAL_NO         "d1e4ec25b8c65d3a"
#elif SENSOR_NO == 2
  #define   SERIAL_NO         "bcc1bf7a23189da2"
#elif SENSOR_NO == 3
  #define   SERIAL_NO         "82e7e6a39518b5a1"
#elif SENSOR_NO == 4
  #define   SERIAL_NO         "e2bb3f9f1759b55c"
#elif SENSOR_NO == 5
  #define   SERIAL_NO         "15266a88024cea84"
#elif SENSOR_NO == 6
  #define   SERIAL_NO         "4695cdf09e695d5b"
#elif SENSOR_NO == 7
  #define   SERIAL_NO         "1c54aeb4bfa76f86"
#elif SENSOR_NO == 8
  #define   SERIAL_NO         "53439104097c4cce"
#endif

// define TX/RX packet size
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

// callback events about RadioEvents defined in the [callback] tab
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );


// modbus setting
softSerial softwareSerial(MAX485_TXD , MAX485_RXD);
ModbusMaster node;

int registerDatas[REGISTER_SIZE];
unsigned int readingCnt = 0;

// time var for tx timing
unsigned long txMillis = 0;

// struct for radio states
typedef enum{
    LOWPOWER,
    RX,
    TX
}States_t;

int16_t txNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi,rxSize;

void setup() {
  setupModbusPin();
  Serial.begin(115200);
  softwareSerial.begin(9600);
  loraSetup();   // in [lora]
  setupModbus();
}

void loop(){
  switch(state){
    case TX:
      processTxJob(); // in [lora]
      break;
		case RX:
		  break;
		case LOWPOWER:
      lowPowerHandler();
  		break;
    default:
      break;
  }
  processReady();
  Radio.IrqProcess( );
}

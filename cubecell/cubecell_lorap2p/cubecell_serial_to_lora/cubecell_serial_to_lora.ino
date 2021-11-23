#include "LoRaWan_APP.h"
#include "Arduino.h"

// Variables abo\\\\\ut LoraDevice Setting, This is not LoraWan solution, 
// only using LoRaWan Library that why the names of variables contain 'LoRaWAN'.
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

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

// device profiles
#define CHANNEL         1
#define VALVE_NO        1
#define DEVICE_TYPE     11 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay, 11: buzzer, 10: fire alarm

// define TX/RX packet size
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

// callback events about RadioEvents defined in the [callback] tab
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

// reserve buffer for Json structs
int relayStatus[CHANNEL] = {};
int relayStatusChange = false;
int switchNo = 1;

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
    boardInitMcu( );
    Serial.begin(9600);
    Serial.setTimeout(100);
    loraSetup();   // in [lora]
    //OnTxDone();
}

void loop(){
  serialCheck();
	switch(state){
		case TX:
        processTxJob();   // in [lora]
		    break;
		case RX:
			  processRxJob();   // in [lora]
		    break;
		case LOWPOWER:
		    break;
    default:
        Serial.println("default state");
        break;
	}
  Radio.IrqProcess( );
}

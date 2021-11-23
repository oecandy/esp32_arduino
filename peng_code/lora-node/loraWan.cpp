/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses OTAA (Over-the-air activation), where where a DevEUI and
 * application key is configured, which are used in an over-the-air
 * activation procedure where a DevAddr and session keys are
 * assigned/generated for use with all further communication.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!

 * To use this sketch, first register your application and device with
 * the things network, to set or generate an AppEUI, DevEUI and AppKey.
 * Multiple devices can use the same AppEUI, but each device has its own
 * DevEUI and AppKey.
 *
 * Do not forget to define the radio type correctly in
 * arduino-lmic/project_config/lmic_project_config.h or from your BOARDS.txt.
 *
 *******************************************************************************/
// Ensure the setting is correct in the LMIC config file located in
// LMIC library/project_config/lmic_project_config.h
// These two defines have no more effect than creating an error if the library setting
// is not correct
#define CFG_us915 1
#define CFG_sx1276_radio 1
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "loraWan.h"

//
// For normal use, we require that you edit the sketch to replace FILLMEIN
// with values assigned by the TTN console. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN to a non-
// working but innocuous value.
//
#ifdef COMPILE_REGRESSION_TEST
# define FILLMEIN 0
#else
# warning "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
# define FILLMEIN (#dont edit this, edit the lines that use FILLMEIN)
#endif

//// This EUI must be in little-endian format, so least-significant-byte
//// first. When copying an EUI from ttnctl output, this means to reverse
//// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
//// 0x70.
//static const u1_t PROGMEM APPEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
//
//// This should also be in little endian format, see above.
//static const u1_t PROGMEM DEVEUI[8]={ 0x54, 0x68, 0x8B, 0x41, 0xFE, 0xAA, 0x97, 0x47  };
//void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
//
//// This key should be in big endian format (or, since it is not really a
//// number but a block of memory, endianness does not really apply). In
//// practice, a key taken from ttnctl can be copied as-is.
//static const u1_t PROGMEM APPKEY[16] = { 0xE9,0x9E,0xF9,0x3B,0x37,0x71,0x34,0xF5,0x93,0x5E,0x75,0x91,0x16,0x47,0x6d,0x4f };
//void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

// LoRaWAN NwkSKey, network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0x8A, 0xBA, 0xD1, 0xE1, 0xDF, 0x73, 0xBF, 0xCC, 0x65, 0xE5, 0xDA, 0x62, 0x9E, 0x77, 0xFF, 0x28 };

// LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = { 0x75, 0x35, 0x06, 0x04, 0xC1, 0x14, 0x81, 0xD1, 0x78, 0xCB, 0xC6, 0x99, 0x07, 0xB8, 0x43, 0x35 };

static const u4_t DEVADDR = 0x0022E7B2;// LoRaWAN NwkSKey, network session key

void os_getArtEui (u1_t* buf) { };
void os_getDevEui (u1_t* buf) { };
void os_getDevKey (u1_t* buf) { };

static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 10;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 5, LMIC_UNUSED_PIN},
};

static volatile transmitionStatus_t txStatus;
static uint8_t * __rxPort;
static uint8_t * __rxBuffer;
static uint8_t * __rxSz;


void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            txStatus = TX_STATUS_ENDING_ERROR;
            break;
        case EV_BEACON_FOUND:
            txStatus = TX_STATUS_UNKNOWN;
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            txStatus = TX_STATUS_UNKNOWN;
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            txStatus = TX_STATUS_UNKNOWN;
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            txStatus= TX_STATUS_JOINING;
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            txStatus= TX_STATUS_JOINED;
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("artKey: ");
              for (int i=0; i<sizeof(artKey); ++i) {
                Serial.print(artKey[i], HEX);
              }
              Serial.println("");
              Serial.print("nwkKey: ");
              for (int i=0; i<sizeof(nwkKey); ++i) {
                Serial.print(nwkKey[i], HEX);
              }
              Serial.println("");
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
            // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     Serial.println(F("EV_RFU1"));
        ||     break;
        */
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            txStatus = TX_STATUS_ENDING_ERROR;
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            txStatus = TX_STATUS_ENDING_ERROR;
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            txStatus =   TX_STATUS_UPLINK_SUCCESS;
            if (LMIC.txrxFlags & TXRX_ACK) {
              txStatus = TX_STATUS_UPLINK_ACKED;
              Serial.println(F("Received ack"));
            }
            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
              if ( __rxPort != NULL) *__rxPort = LMIC.frame[LMIC.dataBeg-1];
              if ( __rxSz != NULL ) *__rxSz = LMIC.dataLen;
              if ( __rxBuffer != NULL ) memcpy(__rxBuffer,&LMIC.frame[LMIC.dataBeg],LMIC.dataLen);
              txStatus = TX_STATUS_UPLINK_ACKED_WITHDOWNLINK;
            }
            // Schedule next transmission
            //os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            txStatus = TX_STATUS_ENDING_ERROR;
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            txStatus = TX_STATUS_ENDING_ERROR;
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            txStatus = TX_STATUS_UNKNOWN;
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            txStatus = TX_STATUS_ENDING_ERROR;
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            txStatus = TX_STATUS_UNKNOWN;
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            txStatus = TX_STATUS_UNKNOWN;
            break;
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            txStatus = TX_STATUS_UNKNOWN;
            break;
    }
}


/**
 * Setup the LoRaWan stack for TTN Europe
 */
void lorawan_setup() {
    txStatus = TX_STATUS_UNKNOWN;
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x13, DEVADDR, nwkskey, appskey);
    LMIC_selectSubBand(1);
    LMIC_setLinkCheckMode(0);
    LMIC.dn2Dr = SF9;
    LMIC_setDrTxpow(DR_SF7,14);
/*
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    LMIC_setLinkCheckMode(0);
    LMIC.dn2Dr = SF9;
    LMIC_setDrTxpow(DR_SF7,14);
*/
}

/**
 * Send LoRaWan frame with ack or not
 * - txPort : port to transmit 
 * - txBuffer : message to transmit
 * - txSz : size of the message to transmit
 * - ack : true for message ack & downlink / false for pure uplink
 *   When Ack is false, the downlink buffer can be set to NULL as rxSz and rPort
 * - rxPort : where to write the port where downlink has been received
 * - rxBuffer : where the downlinked data will be stored
 * - rxSz : size of received data
 */
transmitionStatus_t lorawan_send(uint8_t txPort, uint8_t * txBuffer, uint8_t txSz, bool ack, uint8_t * rxPort, uint8_t * rxBuffer, uint8_t * rxSz){
  // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
        return TX_STATUS_ENDING_ERROR;
    } else {
        txStatus = TX_STATUS_UNKNOWN;
        __rxPort = rxPort;
        __rxBuffer = rxBuffer;
        __rxSz = rxSz;
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(txPort, txBuffer, txSz, ((ack)?1:0));
        // wait for completion
        uint64_t start = millis();
        while ( true ) {
          switch ( txStatus ) {
            case TX_STATUS_UNKNOWN:
            case TX_STATUS_JOINING:
            case TX_STATUS_JOINED:
                os_runloop_once();
                break;
            case TX_STATUS_UPLINK_SUCCESS:
            case TX_STATUS_UPLINK_ACKED:
            case TX_STATUS_UPLINK_ACKED_WITHDOWNLINK:
            case TX_STATUS_UPLINK_ACKED_WITHDOWNLINK_PENDING:
                return txStatus;
          }
          if ( millis() - start > LORA_TIMEOUT_MS ) {
            Serial.println(F("RX_TIMEOUT"));
            lorawan_setup();
            return TX_STATUS_TIMEOUT;
          }
        }        
    }
}

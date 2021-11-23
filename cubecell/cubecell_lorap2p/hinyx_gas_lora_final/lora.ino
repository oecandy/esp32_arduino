void loraSetup(){
  txNumber=0;
  Rssi=0;
// define callback events
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                 LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
  state=TX;
}

void processTxJob(){

  bool status = getGasSensorDatas();

  String msg = "{\"id\":";
  msg += BOARD_ID;
  msg += ",\"CO\":";
  msg += registerDatas[2];
  msg += ",\"CO2\":";
  msg += registerDatas[3];
  msg += ",\"O2\":";
  msg += registerDatas[4];
  msg += ",\"H2S\":";
  msg += registerDatas[5];
  msg += ",\"CH4\":";
  msg += registerDatas[6];
  msg += ",\"readingCnt\":";
  msg += readingCnt;
  msg += ",\"status\":";
  msg += status;
  msg += "}";
  if(readingCnt == 1000) {
    readingCnt = 0;
  }
  msg.toCharArray(txpacket, BUFFER_SIZE);
          
  turnOnRGB(COLOR_SEND,0);
  Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
  Radio.Send((uint8_t *)txpacket, strlen(txpacket));
  state = LOWPOWER;
}

void processReady(){
  unsigned long currentMillis = millis();
  if (currentMillis - txMillis >= INTERVAL_TIME) {
    txMillis = currentMillis;
    state=TX;
  }
}

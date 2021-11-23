void loraSetup(){
  txNumber=0;
  Rssi=0;
// define callback events
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;
// set Radio
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                 LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                 LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                 LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
  state=TX;
}

void processTxJob(){

  unsigned long currentMillis = millis();
  
  StaticJsonDocument<BUFFER_SIZE> doc;
  JsonObject msg = doc.to<JsonObject>();  // {"deviceType":1,"channel":1,"messageType":1,"data":{"valveNo":1,"state":0}}  77+1
    
  msg["deviceType"] = DEVICE_TYPE;
  msg["channel"] = CHANNEL;
  msg["messageType"] = 1;
  msg["data"]["valveNo"] = VALVE_NO;
  msg["data"]["state"] = relayStatus[0];
    
  serializeJson(msg, txpacket);
      
  turnOnRGB(COLOR_SEND,0);
  turnOnRGB(0,0);
    
  Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
  Radio.Send((uint8_t *)txpacket, strlen(txpacket));
  
  state=LOWPOWER;
}

void processRxJob(){
  Serial.println("into RX mode");
  Radio.Rx(0);
  state=LOWPOWER;
  
}

void processReady(){
  unsigned long currentMillis = millis();
  if (currentMillis - txMillis >= INTERVAL_TIME) {
    txMillis = currentMillis;
    state=TX;
  }
}

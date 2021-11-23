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

  bool result = getGasSensorDatas();

  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"device_no\":";
  msg += SENSOR_NO;
  msg += ",\"state\":"; // 1 : 화재 발생, 0 : 정상
  msg += registerDatas[6];
  msg += ",\"temperature\":";
  msg += (float)registerDatas[0]/10;
  msg += ",\"humidity\":";
  msg += (float)registerDatas[1]/10;
  msg += ",\"serialNo\":\"";
  msg += SERIAL_NO;
  msg += "\"}}";
  
  msg.toCharArray(txpacket, BUFFER_SIZE);
          
  turnOnRGB(COLOR_SEND,0);
  if(result){
    Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
    Radio.Send((uint8_t *)txpacket, strlen(txpacket));
    state = LOWPOWER;
  } else {
    state = TX;
  }
}

void processReady(){
  unsigned long currentMillis = millis();
  if (currentMillis - txMillis >= INTERVAL_TIME) {
    txMillis = currentMillis;
    state=TX;
  }
}

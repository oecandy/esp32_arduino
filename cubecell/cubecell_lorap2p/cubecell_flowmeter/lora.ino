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

  bool result = getFlowmeterDatas();
  char hexStr[18];

  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"device_no\":";
  msg += DEVICE_NO;
  msg += ",\"flowmeter\":\"";
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[1], registerDatas[0]); // 실시간 유량
  msg += hexStr;
  msg += "\",\"flowrate\":\"";
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[3], registerDatas[2]); // 실시간 유속
  msg += hexStr;
  msg += "\",\"foward_int\":\"";
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[5], registerDatas[4]); // 정방향 정수
  msg += hexStr;
  msg += "\",\"foward_float\":\"";
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[7], registerDatas[6]); // 정방향 실수
  msg += hexStr;
  msg += "\",\"reverse_int\":\"";
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[9], registerDatas[8]); // 역방향 정수
  msg += hexStr;
  msg += "\",\"reverse_float\":\"";
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[11], registerDatas[10]); // 역방향 실수
  msg += hexStr;
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

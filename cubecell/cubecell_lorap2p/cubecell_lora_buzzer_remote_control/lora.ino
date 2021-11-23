void loraSetup(){
  txNumber=0;
  Rssi=0;
// define callback events
  RadioEvents.TxDone = OnTxDone;
//  RadioEvents.TxTimeout = OnTxTimeout;
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
  state=LOWPOWER;
}

void processTxJob(){

  unsigned long currentMillis = millis();

  String msg = "{\"deviceType\":"; // {"deviceType":11,"channel":1,"messageType":2,"data":{"valveNo":1,"state":0}}  77+1
  msg += DEVICE_TYPE;
  msg += ",\"channel\":";
  msg += CHANNEL;
  msg += ",\"messageType\":";
  msg += 2;
  msg += ",\"data\":{\"valveNo\":";
  msg += switchNo;
  msg += ",\"state\":";
  msg += switchStatus;
  msg += "}}";

  msg.toCharArray(txpacket, BUFFER_SIZE);
  
  //turnOnRGB(COLOR_SEND,0);
    
  Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
  Radio.Send((uint8_t *)txpacket, strlen(txpacket));
  Serial.println("Going to lowpower mode");
  state=LOWPOWER;
}

void onWakeUp1(){
  Serial.println("turn on SWITCH 1");
  delay(500);
  switchNo = DEVICE_NO;
  switchStatus = 1;
  state = TX;
}

void loraSetup(){
  txNumber=0;
  Rssi=0;
// define callback events
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
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
  
  StaticJsonDocument<BUFFER_SIZE> doc;
  JsonObject msg = doc.to<JsonObject>();  // {"deviceType":11,"channel":1,"messageType":2,"data":{"valveNo":1,"state":0}}  77+1
    
  msg["deviceType"] = DEVICE_TYPE;
  msg["channel"] = CHANNEL;
  msg["messageType"] = 2;
  msg["data"]["valveNo"] = switchNo;
  msg["data"]["state"] = switchStatus;
    
  serializeJson(msg, txpacket);
  
  //turnOnRGB(COLOR_SEND,0);
    
  Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
  Radio.Send((uint8_t *)txpacket, strlen(txpacket));
  Serial.println("Going to lowpower mode");
  state=LOWPOWER;
}

void onWakeUp1(){
  Serial.println("turn on SWITCH 1");
  switchNo = 1;
  switchStatus = 1;
  state = TX;
}

void onWakeUp2(){
  Serial.println("turn on SWITCH 2");
  switchNo = 2;
  switchStatus = 1;
  state = TX;
}

void onWakeUp3(){
  Serial.println("turn on SWITCH 3");
  switchNo = 3;
  switchStatus = 1;
  state = TX;
}

void onWakeUp4(){
  Serial.println("turn on SWITCH 4");
  switchNo = 4;
  switchStatus = 1;
  state = TX;
}

//void loopSwitchControl(){
//  switch1State = digitalRead(SWITCH_01);
//  switch1State = digitalRead(SWITCH_02);
//  switch1State = digitalRead(SWITCH_03);
//  switch1State = digitalRead(SWITCH_04);
//  
//  if (switch1State == HIGH) {
//    valveNo = 0;
//    switchStatus = 1;
//    state = TX;
//  } else if (switch2State == HIGH){
//    valveNo = 1;
//    switchStatus = 1;
//    state = TX;
//  } else if (switch3State == HIGH){
//    valveNo = 2;
//    switchStatus = 1;
//    state = TX;
//  } else if (switch4State == HIGH){
//    valveNo = 3;
//    switchStatus = 1;
//    state = TX;
//  }
//}

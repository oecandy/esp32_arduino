void loraSetup(){
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Enable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  LoRa.onReceive(onReceive);
  processTxJob();
  LoRa.receive();
}

void processTxJob() {
  txpacket = "";
  StaticJsonDocument<BUFFER_SIZE> doc;
  JsonObject msg = doc.to<JsonObject>();  // {"deviceType":1,"channel":1,"messageType":1,"data":{"valveNo":1,"state":0}}  77+1

  msg["deviceType"] = DEVICE_TYPE;
  msg["channel"] = CHANNEL;
  msg["messageType"] = 1;
  msg["data"]["valveNo"] = VALVE_NO;
  msg["data"]["state"] = relayStatus[0];

  serializeJson(msg, txpacket);

  Serial.print("sending packet : ");
  Serial.print(txpacket);
  Serial.print(", length : ");
  Serial.println(txpacket.length());
  
  LoRa.beginPacket();                   // start packet
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(txpacket);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}

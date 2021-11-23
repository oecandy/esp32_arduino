void loraSetup(){
  // override the default CS, reset, and IRQ pins (optional)
//  LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN);// set CS, reset, IRQ pin

  if (!LoRa.begin(915E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  
  LoRa.onReceive(onReceive);
  processTxJob();
  LoRa.receive();
  Serial.println("LoRa init succeeded.");
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
  LoRa.print(txpacket);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}

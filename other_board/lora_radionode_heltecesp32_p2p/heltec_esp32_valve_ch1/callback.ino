void onReceive(int packetSize) {
  if (packetSize != 0){
    String rxpacket = "";                 // payload of packet
  
    while (LoRa.available()) {            // can't use readString() in callback, so
      rxpacket += (char)LoRa.read();      // add bytes one by one
    }
    // if message is for this device, or broadcast, print details:
    Serial.println("Message: " + rxpacket);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
  
    DeserializationError error = deserializeJson(doc, rxpacket);
  
    if(error){
      return;
    } else {
      if(doc["messageType"] == 2 && doc["data"]["valveNo"] == VALVE_NO && doc["deviceType"] == DEVICE_TYPE){
        String data = doc["data"];
        DeserializationError error = deserializeJson(doc, data.c_str());
        
        relayStatus[0] = doc["state"];
        valveControl();   // after RxDone, control valve according to 'relayStatus[0]'
        processTxJob();
        LoRa.receive();
      }
    }
  }
}

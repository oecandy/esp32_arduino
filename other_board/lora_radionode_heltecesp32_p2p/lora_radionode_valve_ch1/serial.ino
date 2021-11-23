void serialCheck() {
  if(Serial.available()){
    noInterrupts();
    String inString = Serial.readStringUntil('\n');

    LoRa.beginPacket();                   // start packet
    LoRa.print(inString);                 // add payload
    LoRa.endPacket();                     // finish packet and send it
    interrupts();
  }
}

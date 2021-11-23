void serialCheck() {
  if(Serial.available()){
    noInterrupts();
    char data[77];
    String inString = Serial.readStringUntil('\n');
    inString.toCharArray(data,76);    
    Radio.Send((uint8_t *)data, strlen(data));
    Serial.println(data);
    memset(data,0,77);
    state=LOWPOWER;
    turnOnRGB(COLOR_SEND,0);
    turnOnRGB(0,0);
    interrupts();
  }
}

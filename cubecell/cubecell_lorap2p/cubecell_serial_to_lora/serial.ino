void serialCheck() {
  if(Serial.available()){
    noInterrupts();
    char data[250];
    String inString = Serial.readStringUntil('\n');
    inString.toCharArray(data,250);    
    Radio.Send((uint8_t *)data, strlen(data));
    Serial.println(data);
    state=LOWPOWER;
    turnOnRGB(COLOR_SEND,0);
    turnOnRGB(0,0);
    interrupts();
  }
}

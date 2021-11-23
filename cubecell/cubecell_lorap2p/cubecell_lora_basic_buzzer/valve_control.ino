void valveControl(){
  if(relayStatus[0]==1){
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("");
    Serial.printf("valve[%u]'s turned on!\n",VALVE_NO);
    state=TX;
  }
}

void switchControl(){
  digitalWrite(RELAY_PIN, HIGH);
  controlStep = 1;
  controlMillis = millis();
  Serial.printf("valve[%u]'s turned on!\n",VALVE_NO);
  state=TX;
}

void controlLoop(){
  unsigned long currentMillis = millis();
  if(currentMillis - controlMillis >= CONTROL_TIME && relayStatus[0] == 1){
    controlStep = 0;
    digitalWrite(RELAY_PIN, LOW);
    relayStatus[0] = 0;
    Serial.printf("valve[%u]'s turned off!\n",VALVE_NO);
    state=TX;
  }
}

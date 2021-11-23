void valveControl(){
  if(relayStatus[0]==1){
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("");
    Serial.printf("valve[%u]'s turned on!\n",VALVE_NO);
    state=TX;
  } else if(relayStatus[0]==0) {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("");
    Serial.printf("valve[%u]'s turned off!\n",VALVE_NO);
    state=TX;
  }
}

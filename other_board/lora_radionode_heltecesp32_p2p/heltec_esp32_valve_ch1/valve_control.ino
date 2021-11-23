void valveControl(){
  if(relayStatus[0]==1){
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("valve[" + String(VALVE_NO) + "]'s turned on!");
  } else if(relayStatus[0]==0) {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("valve[" + String(VALVE_NO) + "]'s turned off!");
  }
}

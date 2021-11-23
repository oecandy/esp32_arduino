void loopValveControll(){
  if(send_command){
    if(command_status[0]){
      digitalWrite(RELAY_1_PIN, HIGH);
      delay(1000);
      digitalWrite(RELAY_2_PIN, HIGH);
      send_command = false;
    } else {
      digitalWrite(RELAY_2_PIN, LOW);
      delay(1000);
      digitalWrite(RELAY_1_PIN, LOW);
      send_command = false;
    }
  }
}

void loopPubDeviceData(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - latestMillis >= INTERVAL_TIME) {
      latestMillis = currentMillis;
      mqtt_publish_data();
  }
}

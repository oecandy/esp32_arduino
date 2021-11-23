void sensor_loop(){
  unsigned long currentMillis = millis();
  
  if (currentMillis - loopMillis >= INTERVAL_TIME) {
    loopMillis = currentMillis;
    read_flowmeter_value();
    parse_hex_string();
    sendMessage();
  }
}

void parse_hex_string(){
  char hexStr[18];  
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[1], reg_vals[0]); // 실시간 유량
  Serial.println(hexStr);

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[3], reg_vals[2]); // 실시간 유속
  Serial.println(hexStr);
  
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[5], reg_vals[4]); // 정방향 정수
  Serial.println(hexStr);

  
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[7], reg_vals[6]); // 정방향 소수
  Serial.println(hexStr);
  
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[9], reg_vals[8]); // 역방향 정수
  Serial.println(hexStr);

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[11], reg_vals[10]); // 실시간 유량
  Serial.println(hexStr);
  
}

void reset_loop(){
  unsigned long currentMillis = millis();
  if(resetFlag){
    if (currentMillis - resetMillis >= RESET_TIME) {
      ESP.restart();
    }
  } else {
    resetMillis = currentMillis;
    resetFlag = 1;
  }
}

void rc_sensor_setup(){
  recvSensor.enableReceive(RF_RX_PIN);
}

void recive_rc_message(){
  if(sensorState){ // 화재 경보 상태 일때 일정 시간(10초) 지나면 상태 다시 0으로 돌림  
    unsigned long currentMillis = millis();
    if(currentMillis - sensorMillis >= INTERVAL_TIME){
       sensorState = 0;
    }
  }
  
  if (recvSensor.available()) {
    if (recvSensor.getReceivedValue()==codeSensorAlarm){ // 알람센서 433 신호 받으면 상태 1로 변경하고 mqtt 메세지 전송
      sensorState = 1;
      sensorMillis = millis();
      mqtt_publish_data();
    }
    Serial.println(recvSensor.getReceivedValue());
    recvSensor.resetAvailable();
  }
}

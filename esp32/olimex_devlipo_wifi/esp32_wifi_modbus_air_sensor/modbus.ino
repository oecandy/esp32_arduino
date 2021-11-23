void modbus_setup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cb_write(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  char statusStr[6];
  snprintf(statusStr, sizeof(statusStr), "%02x", event);
  String status = statusStr;
  if(status == "00"){
    gasSensor.co = (float) registerDatas[0]/100;
    gasSensor.co2 = registerDatas[1];
    gasSensor.o2 = (float) registerDatas[2]/100;
    gasSensor.h2s = (float) registerDatas[3]/100;
    gasSensor.ch4 = registerDatas[4];

  }
  return true;
}

void read_sensor_registers(){
  if(!mb.slave()){
    mb.readHreg(SLAVE_ID,2,registerDatas,REGISTER_SIZE,cb_write);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}


void sensor_loop(){
  unsigned long currentMillis = millis();
  
  if (currentMillis - intervalMillis >= INTERVAL_TIME) {
    intervalMillis = currentMillis;
    read_sensor_registers();
    send_message();
  }
}

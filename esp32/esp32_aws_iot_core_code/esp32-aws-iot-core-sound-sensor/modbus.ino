void modbusSetup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  char status_str[6];
  snprintf(status_str, sizeof(status_str), "%02x", event);
  String status = status_str;
  if(status == "00"){
    sound_sensor_datas[coll_cnt] = (float)reg_vals[0]/10;
    Serial.println(sound_sensor_datas[coll_cnt]);
    coll_cnt++;
    
  }
  return true;
}

void readSoundSensor(){
  if(!mb.slave()){
    mb.readHreg(SOUND_SLAVE_ID,0,reg_vals,REGISTER_SIZE,cbWrite);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}

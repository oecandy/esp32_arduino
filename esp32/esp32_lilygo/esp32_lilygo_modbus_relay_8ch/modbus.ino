void loop_modbus(){
  if (!mb.slave()) {
    mb.readCoil(idx_cnt, 0, coils, 1);
    Serial.print("valve_");
    Serial.println(idx_cnt);
  }
  mb.task();
  yield();
  
  valve_status[idx_cnt] = coils[0];

  if(idx_cnt == 7){
    idx_cnt = 0;
    mqtt_publish_data();
  } else {
    idx_cnt ++;
  }
}

void loop_write_coil(){
  for(int i = 0; i < RELAY_TYPE; i ++){
    controll_valve(i, command_status[i]); 
  }
}

void modbus_setup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
//  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

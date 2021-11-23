void modbus_setup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}


void read_water_quality_value(){
  if(!mb.slave()){
    mb.readHreg(WQ_SLAVE_ID,1,reg_wq_vals,REGISTER_SIZE,cbWrite);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}

void read_water_level_value(){
  if(!mb.slave()){
    mb.readHreg(WL_SLAVE_ID,4,reg_wl_vals,REGISTER_SIZE,cbWrite);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}

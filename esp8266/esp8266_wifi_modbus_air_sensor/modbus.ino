void modbus_setup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

void loop_read_regs_air_sensor(){
  int slave_id = 1;
  if(!mb.slave()){
    mb.readHreg(slave_id,1,air_reg_vals,2,cbWrite);
    while(mb.slave()){
      mb.task();
      delay(10);
      air_sensor.temp = air_reg_vals[0];
      air_sensor.humi = air_reg_vals[1];
    }
    yield();
  }
}

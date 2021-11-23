void modbus_setup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}


void read_flowmeter_value(){
  if(!mb.slave()){
    mb.readHreg(SLAVE_ID,0,reg_vals,REGISTER_SIZE,cbWrite);
    while(mb.slave()){
      mb.task();
    }
    for(int i = 0; i < REGISTER_SIZE; i ++){
      Serial.print("register[");
      Serial.print(i);
      Serial.print("] : ");
      Serial.println(reg_vals[i]);
    }
    yield();
  }
}

void modbusSetup(){
  #if defined(ESP8266)
    mb.begin(&S);
  #elif defined(ESP32)
    mb.begin(&Serial2,RE_DE);
  #endif
  
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

void loopReadSensorRegs(){
  
  if(!mb.slave()){
    mb.readHreg(SLAVE_ID,0,registerDatas,10,cbWrite);
    while(mb.slave()){
      mb.task();
    }
    yield();
    sendMessage();
  }
}

void modbusSetup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWriteGps(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  char statusStr[6];
  snprintf(statusStr, sizeof(statusStr), "%02x", event);
  String status = statusStr;
  if(status == "00"){
    gpsData.longitude = convert_modbus_data(registerDatas[0], registerDatas[1]);
    gpsData.latitude = convert_modbus_data(registerDatas[3], registerDatas[4]);
  }
  return true;
}

void loopReadGpsSensorRegs(){
  
  if(!mb.slave()){
    mb.readHreg(GPS_SLAVE_ID,208,registerDatas,REGISTER_SIZE,cbWriteGps);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}

float convert_modbus_data(uint16_t val1, uint16_t val2){
  
  char hexstring[18];
  snprintf(hexstring, sizeof(hexstring), "%04X%04X", val1, val2);

  union {
        uint32_t i;
        float f;
  } data;

  data.i = strtoul(hexstring, NULL, 16);

  Serial.println(hexstring);
  Serial.println(data.i);
  Serial.println(data.f, 7);
  
  return data.f;
}

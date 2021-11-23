void modbusSetup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  char statusStr[6];
  snprintf(statusStr, sizeof(statusStr), "%02x", event);
  String status = statusStr;
  if(status == "00"){
    weatherData.temperature = (float)registerDatas[1]/10;
    weatherData.humidity = (float)registerDatas[2]/10;
    weatherData.rain = (float)registerDatas[6]/10;
    weatherData.windDir = registerDatas[3];
    weatherData.windSpeed = (float)registerDatas[4]/10;
    weatherData.radiation = (float)registerDatas[11]/10;
    weatherData.uv = registerDatas[9];

    setRainArr();
  }
  return true;
}

void loopReadSensorRegs(){
  
  if(!mb.slave()){
    mb.readHreg(SLAVE_ID,0,registerDatas,10,cbWrite);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}

void setRainArr(){
  
  hoursRainArr[hourRainCnt] = (float)registerDatas[6]/10;
  hourRainCnt++;
  
  if(HOUR_ARR_SIZE <= hourRainCnt){
    hourRainCnt = 0;
  }

  daysRainArr[dayRainCnt] = (float)registerDatas[6]/10;
  dayRainCnt ++;
  
  if(DAY_ARR_SIZE <= dayRainCnt){
    dayRainCnt = 0;
  }
}

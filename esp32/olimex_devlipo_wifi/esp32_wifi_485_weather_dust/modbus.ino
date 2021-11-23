void modbusSetup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWriteWeather(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  char statusStr[6];
  snprintf(statusStr, sizeof(statusStr), "%02x", event);
  String status = statusStr;
  if(status == "00"){
    weatherData.temperature = (float)(registerDatas[1] - 400)/10;
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

bool cbWriteDust(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  char statusStr[6];
  snprintf(statusStr, sizeof(statusStr), "%02x", event);
  String status = statusStr;
  if(status == "00"){
    if(registerDatas[1] > 32767){
      dustData.pm25 = 0;
    } else {
      dustData.pm25 = registerDatas[1]; // 0x0012 0-1000μg/m3 PM2.5
    }

    if(registerDatas[1] > 32767){
      dustData.pm10 = 0;
    } else {
      dustData.pm10 = registerDatas[2]; // 0x0013 0-1000μg/m3 PM10
    }
  }
  return true;
}

void loopReadWeatherSensorRegs(){
  
  if(!mb.slave()){
    mb.readHreg(WEATHER_SLAVE_ID,0,registerDatas,REGISTER_SIZE,cbWriteWeather);
    while(mb.slave()){
      mb.task();
    }
    yield();
  }
}

void loopReadDustSensorRegs(){
  
  if(!mb.slave()){
    mb.readHreg(DUST_SLAVE_ID,17,registerDatas,REGISTER_SIZE,cbWriteDust);
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

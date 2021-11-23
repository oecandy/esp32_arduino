void wifi_setup() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(count < 5){
      count ++;
    } else {
      count= 0;
      WiFi.reconnect();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sensorLoop(){
  unsigned long currentMillis = millis();
  if (currentMillis - loopMillis >= INTERVAL_TIME) {
    loopMillis = currentMillis;
    loopReadWeatherSensorRegs();
    getHourRain();
    getDayRain();
    delay(1000);
    loopReadDustSensorRegs();
    sendMessage();
  }
}

void getHourRain(){
  float hourRain = 0;
  for(int i = 0; i < HOUR_ARR_SIZE; i ++){
    hourRain += hoursRainArr[i];
  }

  weatherData.hourRain = hourRain;
}

void getDayRain(){
  float dayRain = 0;
  for(int i = 0; i < DAY_ARR_SIZE; i ++){
    dayRain += daysRainArr[i];
  }

  weatherData.dayRain = dayRain;
}

void sensorLoop(){
  unsigned long currentMillis = millis();
  loopReadSensorRegs();
  if (currentMillis - loopMillis >= INTERVAL_TIME + timeFlag) {
    loopMillis = currentMillis;
    sendMessage();
    timeFlag = getTimeFlag();
  }
}

int getTimeFlag(){
  return random(1000, 3000);
}

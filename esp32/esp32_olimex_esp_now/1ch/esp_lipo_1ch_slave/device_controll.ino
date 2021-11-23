void ICACHE_RAM_ATTR getFlow();

void getFlow ()
{
    ++ flowCount;    //  low가 인식, high가 미인식
}

void flow_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - flowMillis >= INTERVAL_TIME + timeFlag) {
      flowMillis = currentMillis;
      sendPayload();
      Serial.println("flowCount is: ");
      Serial.println(flowCount);
      flowCount = 0;
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), getFlow, FALLING);
}

void valveControll(){
  if(relayStatusChange)
  { 
    if(relayStatus[0]){
      digitalWrite(RELAY_PIN, HIGH);
      
    } else if(relayStatus[0] == 0){
      digitalWrite(RELAY_PIN, LOW);
    }
    relayStatusChange = false;
  }
}

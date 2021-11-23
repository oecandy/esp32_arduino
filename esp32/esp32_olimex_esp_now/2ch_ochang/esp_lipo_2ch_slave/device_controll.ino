void ICACHE_RAM_ATTR getFlow();

void getFlow1 ()
{
    ++ flowCount1;    //  low가 인식, high가 미인식
}

void getFlow2 ()
{
    ++ flowCount2;    //  low가 인식, high가 미인식
}

void flow_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - flowMillis >= INTERVAL_TIME + timeFlag) {
      flowMillis = currentMillis;
      Serial.println("flowCount1 is: ");
      Serial.println(flowCount1);
      Serial.println("flowCount2 is: ");
      Serial.println(flowCount2);
      sendPayload();
      flowCount1 = 0;
      flowCount2 = 0;
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_1_PIN), getFlow1, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_2_PIN), getFlow2, FALLING);
}

void valveControll(){
  int relay;
  switch(switchNo){
    case 1 :
      relay = relayStatus[0] == 1 ? 1 : 0;
      digitalWrite(RELAY_1_PIN, relay);
      break;
    case 2 :
      relay = relayStatus[1] == 1 ? 1 : 0;
      digitalWrite(RELAY_2_PIN, relay);
      break;
  }
}

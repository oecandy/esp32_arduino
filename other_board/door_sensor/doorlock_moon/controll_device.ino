void pinSetup(){
  pinMode (RELAY_1_PIN, OUTPUT);
  pinMode (RELAY_2_PIN, OUTPUT);
}

// Door open for 30secs
void doorOpen(){
  doorIntervalMillis = millis();
  doorStep = 1;
}

void doorClose(){
  doorIntervalMillis = millis();
  doorStep = 2;
}

void sirenOn(){
  sirenIntervalMillis = millis();
  sirenStep = 1;
}

void sirenOff(){
  sirenStep = 4;
}

void loopDoorDevice(){
  unsigned long currentMillis = millis();
  switch(doorStep){ // 0: 동작 X, 1: 3초간 울림, 2: 1초 동작 X, 3: repeat count ++ and 판별, 4: 강제 종료s
    case 0:
      break;
    case 1:
      if(currentMillis - doorIntervalMillis >= DOOR_OPEN_TIME){
        digitalWrite(RELAY_1_PIN,LOW);
        doorStep = 0;
        relayStatus[0] = 0;
        Serial.println("door step 1 end");
      } else {
        if(relayStatus[0] == 0){
          digitalWrite(RELAY_1_PIN,HIGH);
          Serial.println("door step 1 start");
          relayStatus[0] = 1;
        }
      }
      break;
    case 2:
      digitalWrite(RELAY_1_PIN,LOW);
      Serial.println("door step 2");
      doorStep = 0;
      break;
  }
}


void loopSirenDevice(){
  unsigned long currentMillis = millis();
  switch(sirenStep){ // 0: 동작 X, 1: 3초간 울림, 2: 1초 동작 X, 3: repeat count ++ and 판별, 4: 강제 종료s
    case 0:
      break;
    case 1:
      if(currentMillis - sirenIntervalMillis >= BEEP_TIME){
        digitalWrite(RELAY_2_PIN,LOW);
        sirenStep++;
        sirenIntervalMillis = currentMillis;
        Serial.println("siren step 1 end");
      } else {
        if(relayStatus[1] == 0){
          digitalWrite(RELAY_2_PIN,HIGH);
          Serial. println("Siren HIGH");
          relayStatus[1] = 1;
          Serial.println("siren step 1 start");
        }
        
      }
      break;
    case 2:
      if(currentMillis - sirenIntervalMillis >= BEEP_INTERVAL){
        sirenStep++;
        Serial.println("siren step 2 end");
      }
      break;
    case 3:
      if(repeatCnt < SIREN_REPEAT){
        Serial.print("siren step 3 count");
        Serial.println(repeatCnt);
        repeatCnt++;
        sirenStep = 1;
        sirenIntervalMillis = currentMillis;
      } else {
        Serial.println("siren step 3 end");
        relayStatus[1] = 0;
        sirenStep = 0;
        repeatCnt = 0;
      }
      break;
    case 4:
      digitalWrite(RELAY_2_PIN,LOW);
      sirenStep = 0;
      repeatCnt = 0;
      break;
  }
}

void loopSensorMessage(){
  unsigned long currentMillis = millis();
  if (currentMillis - sendIntervalMillis >= INTERVAL_TIME) {
      sendIntervalMillis = currentMillis;
      sendPayload();
  }
}

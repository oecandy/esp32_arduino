void start_Valve_step (void)
{
  if(valveState != valveCommand)      // 밸브가 이전 상태와 다르게 동작하라는 명령이 오면
  {
    if(!valveStep)     // 현재 밸브가 동작 중이 아니면
      valveStep = 1;                  // 밸브 동작 시작
  }
}

void Start_Valve_OFF (void)
{
    valveCommand = 0;

    if(valveState)                  // 밸브가 켜져있으면
        valveStep = 1;              // 밸브 동작
}

void Init_Valve (void)
{
    digitalWrite(VALVE_IN1, LOW);     // Coast;
    digitalWrite(VALVE_IN2, LOW);
}

void Valve_Operation (byte sel)
{
    if(sel)
    {
        digitalWrite(VALVE_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
        digitalWrite(VALVE_IN2, HIGH);
    }
    else
    {
        digitalWrite(VALVE_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
        digitalWrite(VALVE_IN2, LOW);
    }
}

void relayOn(){
  delay(100);
  digitalWrite(SEQUENCE_SWITCH,LOW);
  delay(100);
  digitalWrite(SEQUENCE_SWITCH,HIGH);
}

void relayOff(){
  delay(100);
  digitalWrite(SEQUENCE_SWITCH,LOW);
}

void relayGeneratorOn(){
  delay(100);
  digitalWrite(VALVE_PWR_ON,LOW);
  delay(100);
  digitalWrite(VALVE_PWR_ON,HIGH);
}

void relayGeneratorOff(){
  delay(100);
  digitalWrite(VALVE_PWR_ON,LOW);
}

void control_Valve (unsigned long waitTime)
{
    switch(valveStep)
    {
        case 1:
            digitalWrite(VALVE_PWR_SW, HIGH);    // ENABLE(ON) Active High
            digitalWrite(VALVE_PWR_ON, HIGH);    // Pull high to enable the device, pull low to disable the device.
            checkVtime = millis();
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("vS=");
            Serial.println(valveStep);
        #endif
            break;

        case 2:
            if(waitTime - checkVtime >= TURN_ON_TIME)      // Wait Turn On Time
                ++ valveStep;
            break;

        case 3:
      Valve_Operation(valveCommand);
            checkVtime = millis();
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("vS=");
            Serial.println(valveStep);
        #endif
            break;

        case 4:
            if(waitTime - checkVtime >= DELAY_TIME)     // Operating pulse width
                ++ valveStep;
            break;

        case 5:
            digitalWrite(VALVE_PWR_SW, LOW);
            digitalWrite(VALVE_PWR_ON, LOW);
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("vS=");
            Serial.println(valve2step);
        #endif
            break;

        case 6:
      Init_Valve();
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("v1s=");
            Serial.println(valve2step);
        #endif
            break;

        case 7:
            valveState = valveCommand;
            ++ valveStep;
        #if DEBUG_MODE
            Serial.print("v1s=");
            Serial.print(valve1step);
            Serial.print(", v2s=");
            Serial.println(valve2step);
        #endif
            break;

        case 8:
            valveStep = 0;
        #if DEBUG_MODE
            Serial.print("v1s=");
            Serial.print(valve1step);
            Serial.print(", v2s=");
            Serial.println(valve2step);
        #endif
            break;
        default: break;
    }
}

uint8_t valveOFF = 0;
void Check_Valve (void)             // 밸브가 켜져 있으면 문제가 되므로 일정 시간 이후에 꺼지도록하는 함수
{
// check list - 10 변경하여 통신 끊김 빨리 재현시켜 밸브가 꺼지는지 확인
// 1. 밸브가 켜져 있고, 통신이 끊기면 꺼지는지 확인
// 2. 통신이 끊기고 다시 접속하면 정상 동작 (ON, OFF) 확인
    if( (check_com_count > 10) && (!valveOFF) && (valveState) && (!valveStep) )    // 30번 전송하는 동안 통신이 끊기면 (10s * 10 = 100s)
    {
        valveOFF = 1;
        Start_Valve_OFF();

        while (valveStep)  // confirm을 10초씩 대기하는 경우가 생기므로
        {                                   // 다른 함수 진입하지 않고 while에서 밸브 꺼지도록 처리
            unsigned long countMillis = millis();
            control_Valve(countMillis);
        }
    }
}
/* --------------------------------------------------------------------------------------------------------------------------- */

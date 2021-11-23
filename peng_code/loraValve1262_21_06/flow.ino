

void getFlow (void)
{
    FL.ulong_val ++;    //  low가 인식, high가 미인식

    Serial.println("flow counted");
}

void flow_sequence (unsigned long readTime)
{
    switch(flowStep)
    {
        case 1:
//            FL.ulong_val = 0;
            digitalWrite(FLOW_PWR_ON, HIGH); // Flow Sensor power ON
            Wire.begin();
            attachInterrupt(FLOW_IN,getFlow,FALLING); // example
            // attachInterrupt(digitalPinToInterrupt(FLOW_IN), getFlow, FALLING);    // Flow Sensor interrupt ON
            checkFtime = millis();    // 현재 시간 저장
            ++ flowStep;            // 다음 단계로 이동

        #if DEBUG_MODE
            Serial.println("f1");
        #endif
            break;

        case 2:
            if(readTime - checkFtime >= FLOW_TIME)    // 설정 시간만큼 유량값 측정
                ++ flowStep;
            break;

        case 3:
            detachInterrupt(FLOW_IN);        // 유량 인터럽트 정지
            Wire.end();
            digitalWrite(FLOW_PWR_ON, LOW); // Flow power OFF
            flowStep = 0;
        #if DEBUG_MODE
            Serial.println("m3");
        #endif
        #if CHECK_VALUE
            Serial.print("flow = ");
            Serial.println(FL.ulong_val);
        #endif
            break;

        default: break;
    }
}

void start_flow_step (void)
{
  flowStep = 1;
}
/* --------------------------------------------------------------------------------------------------------------------------- */

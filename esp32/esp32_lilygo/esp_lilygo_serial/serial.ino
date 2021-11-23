void comunicateSerialData(){
  if(Serial2.available()){
    char data = Serial2.read();
    if(data == 'N'){
      recvStart = true;
      recvCount = 0;
      Serial.println("data recieve started!");
    }
    
    Serial.println(data);

    if(recvStart){
      resultStr += data;
      recvCount++;
    }

    if(recvCount == 10){
      recvStart == false;
      recvCount = 0;
      resultStr.trim();
      Serial.print("full data recieved :: ");
      Serial.print(resultStr);
      setPublishData();
      mqttPublishValveData();
      
      resultStr = "";
    }
  }
}

void setPublishData(){
  String valveNo = resultStr.substring(1,3);
  String relayState = resultStr.substring(4,5);
  String flowCount = resultStr.substring(6);

  valveData.valveNo = atoi(valveNo.c_str());
  valveData.relayState = atoi(relayState.c_str());
  valveData.flowCount = atoi(flowCount.c_str());
}

void commandSendSerial(){
  if(commandFlag){
      Serial2.write(cmdBuf);
      commandFlag = false;
    }
}

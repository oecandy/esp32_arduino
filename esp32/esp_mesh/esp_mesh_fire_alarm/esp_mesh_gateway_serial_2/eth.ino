void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void comunicateSerialData(){
  if(Serial2.available()){
    String data = Serial2.readString();
    data.trim();
    pubDeviceData(data);
//    if(data == 'E'){
//      recvStart == false;
//      resultStr.trim();
//      Serial.print("full data recieved :: ");
//      Serial.print(resultStr);
//      
//      resultStr = "";
//    }
//    
//    if(recvStart){
//      resultStr += data;
//      Serial.println(data);
//    }
//    
//    if(data == 'S'){
//      recvStart = true;
//      Serial.println("data recieve started!");
//    }
  }
}

void pubDeviceData(String msg){
  DeserializationError error = deserializeJson(doc, msg.c_str());
  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    if(doc["messageType"] == 1){
      String result;
      serializeJson(doc["data"], result);
      String topic;
      switch((int)doc["deviceType"]){
        case 1:
          topic = VALVE_1CH_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 2:
          topic = VALVE_2CH_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 3:
          topic = VALVE_6CH_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 5:
          topic = VIB_DATA_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
      }
    }
  }
}

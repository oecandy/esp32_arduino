void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(topic);
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
    Serial.println((int)root["sleep"]);
    Serial.println((int)root["gain"]);
    sleepTime = root["sleep"];
    gain = root["gain"];
  } else {
    Serial.println("parseObject() failed");
  }
}

void wifiMqttInit() {
  //WiFi.onEvent(WiFiEvent);
  
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(maxMqttBufferSize);
  mqttClient.subscribe(confTopic);
}

void mqttPublishMessage(String jsonData){
  switch(axisIdx){
      case 1:
        mqttClient.publish (xAxisTopic, jsonData.c_str());
        Serial.println("x data send");
        break;
      case 2:
        mqttClient.publish (yAxisTopic, jsonData.c_str());
        Serial.println("y data send");
        break;
      case 3:
        mqttClient.publish (zAxisTopic, jsonData.c_str());
        Serial.println("z data send");
        delay(100);
        mqttPublishTemp();
        delay(100);
        mqttPublishHealthCheck();
        valNo ++;
        Serial.print("next val no : ");
        Serial.println(valNo);
        break;
      default:
        break;
    }
}

void mqttPublishTemp(){
  String jsonData = "{\"val_no\":";
  jsonData += valNo;
  jsonData += ",\"eng_min\":\"-16\",\"eng_max\":\"16\",\"datacnt\":";
  jsonData += 1;
  jsonData += ",\"gain\":";
  jsonData += gain;
  jsonData += ",\"val\":[";
  jsonData += (float)targetTemp/100;
  jsonData += "]}";
  String nodeTopic = tempTopic;
  mqttClient.publish (nodeTopic.c_str(), jsonData.c_str());
}

void mqttPublishHealthCheck(){
  String jsonData = "{\"rssi\":";
  jsonData += targetRssi;
  jsonData += ",\"mac_address\":\"";
  jsonData += targetMacAddress;
  jsonData += "\",\"serial_no\":\"";
  jsonData += targetSerial;
  jsonData += "\",\"battery\":\"";
  jsonData += targetBattery;
  jsonData += "\",\"bootConut\":\"";
  jsonData += targetBootCnt;
  jsonData += "\",\"eng_min\":\"-16\",\"eng_max\":\"16\",\"datacnt\":";
  jsonData += dataCnt;
  jsonData += ",\"gain\":";
  jsonData += gain;
  jsonData += ",\"sleep\":";
  jsonData += sleepTime;
  jsonData += "}";
  String nodeTopic = connTopic;
  nodeTopic += "/";
  nodeTopic += targetSerial;
  mqttClient.publish (nodeTopic.c_str(), jsonData.c_str());
}

void mqttPublishConfigInfo(){
  String message = "{\"serial_no\":\"FS1001_1209B\"";
  message += ",\"sleep\":";
  message += sleepTime;
  message += ",\"gain\":";
  message += gain;
  message += "}";
  mqttClient.publish(confTopic, message.c_str(), true);
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESPMODBUS-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe(confTopic);
//       Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

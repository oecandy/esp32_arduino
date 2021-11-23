void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
    commandData.valveNo = root["valveNo"];
    commandData.relayState = root["state"];
    snprintf(cmdBuf, 20, "N%02dV%d", commandData.valveNo, commandData.relayState);
    
    commandFlag = true;
  } else {
    Serial.println("parseObject() failed");
  }
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
      // Once connected, publish an announcement...
      mqttClient.publish(connTopic, "connected");
      mqttPublishHealthCheck();
      mqttClient.subscribe(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void wifiMqttInit() {
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(MAX_MQTT_BUFFER_SIZE);
}

void mqttPublishHealthCheck(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"mac\":\"%s\",\"serialNo\":\"%s\",\"heap\":%ld}", ETH.localIP().toString().c_str(), ETH.macAddress().c_str(), SERIAL_NO, ESP.getFreeHeap());
  mqttClient.publish (pingTopic, message);
}

void mqttPublishValveData(){
  
  String jsonData = "{\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"valveNo\":";
  jsonData += valveData.valveNo;
  jsonData += ",\"flow\":";
  jsonData += valveData.flowCount;
  jsonData += ",\"state\":";
  jsonData += valveData.relayState;
  jsonData += "}";
  
  mqttClient.publish (dataTopic, jsonData.c_str());
  
}

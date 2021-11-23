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
    Serial.println((int)root["valveNo"]);
    Serial.println((int)root["switchNo"]);
    Serial.println((int)root["state"]);
    valveData.valve_no = (int)root["valveNo"];
    valveData.switch_no = (int)root["switchNo"];
    valveData.relay_state = (int)root["state"];
    sendData();
    
  } else {
    Serial.println("parseObject() failed");
  }
}

void wifiMqttInit() {
  //WiFi.onEvent(WiFiEvent);
  
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(maxMqttBufferSize);
  mqttClient.subscribe(SUB_TOPIC);
}

void mqttPublishMessage(){
  String jsonData = "{\"valveNo\":";
  jsonData += myData.valve_no;
  jsonData += ",\"SW01\":{\"state\":";
  jsonData += myData.relay_state[0];
  jsonData += ",\"flow\":";
  jsonData += myData.flow_count[0];
  jsonData += "},\"SW02\":{\"state\":";
  jsonData += myData.relay_state[1];
  jsonData += ",\"flow\":";
  jsonData += myData.flow_count[1];
  jsonData += "}}";
  
  mqttClient.publish(DATA_TOPIC, jsonData.c_str());
}

void mqttPublishHealthCheck(){
  String jsonData = "{\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"channel\":";
  jsonData += CHANNEL;
  jsonData += ",\"macAddress\":\"";
  jsonData += WiFi.macAddress();
  jsonData += "\"}";
  mqttClient.publish (PING_TOPIC, jsonData.c_str());
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
      mqttClient.subscribe(SUB_TOPIC);
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

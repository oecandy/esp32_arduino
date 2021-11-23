void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(topic);
  DeserializationError error = deserializeJson(doc, (char*)payload);
  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    
  }
}

void wifiMqttInit() {
  //WiFi.onEvent(WiFiEvent);
  
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(maxMqttBufferSize);
}

void mqttPublishData(){
  String jsonData = "{\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"waterLevel\":";
  jsonData += myData.water_level;
  jsonData += ",\"waterQuality\":";
  jsonData += myData.water_quality;
  jsonData += "}";
  
  mqttClient.publish (DATA_TOPIC, jsonData.c_str());
}

void mqttPublishHealthCheck(){
  String jsonData = "{\"serialNo\":\"";
  jsonData += SERIAL_NO;
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
      mqttPublishHealthCheck();
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

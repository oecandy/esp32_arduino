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
    String result;
    doc["messageType"] = 2;
    char *ptr = strtok(topic, "/");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
    ptr = strtok(NULL, "/");
    ptr = strtok(NULL, "/");
    Serial.println(ptr);

    if(String(ptr) == "relay"){
      ptr = strtok(NULL, "/");
      Serial.println(ptr);
      if(String(ptr) == VALVE_1CH_SERIAL_NO){
        doc["deviceType"] = 1;
      } else if (String(ptr) == BUZZER_SERIAL_NO){
        doc["deviceType"] = 10;
      }
    } else if(String(ptr) == "relay_v3"){
      doc["deviceType"] = 2;
    } else if(String(ptr) == "relay_v5"){
      doc["deviceType"] = 3;
    }
    
    serializeJson(doc, result);
    Serial.println(result);
    Serial2.println(result);
  }
}

void wifiMqttInit() {
  
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(maxMqttBufferSize);
  mqttClient.subscribe(VALVE_1CH_SUB_TOPIC);
  mqttClient.subscribe(VALVE_2CH_SUB_TOPIC);
  mqttClient.subscribe(VALVE_6CH_SUB_TOPIC);
  mqttClient.subscribe(BUZZER_SUB_TOPIC);
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
  mqttClient.subscribe(VALVE_1CH_SUB_TOPIC);
  mqttClient.subscribe(VALVE_2CH_SUB_TOPIC);
  mqttClient.subscribe(VALVE_6CH_SUB_TOPIC);
  mqttClient.subscribe(BUZZER_SUB_TOPIC);
      
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqttClient.state());
      Serial.println("reboot");
      ESP.restart();
    }
  }
}

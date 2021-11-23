void mqtt_setup(){
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  client.setBufferSize(MAX_MQTT_BUFFER_SIZE);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESPMODBUS-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish(CONNECT_TOPIC, "connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// 구독 없어서 실제로는 동작하지 않는 함수
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
  } else {
  }
}

void mqttPublishHealthCheck(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"rssi\":\"%s\",\"serialNo\":\"%s\",\"heap\":%ld}", WiFi.localIP().toString().c_str(), String(WiFi.RSSI()).c_str(), SERIAL_NO, ESP.getFreeHeap());
  client.publish (PING_TOPIC, message);
}

void mqtt_publish_data(){
  String jsonData = "{\"state\":";
  jsonData += sensorState;
  jsonData += "}";
  client.publish (PUB_TOPIC, jsonData.c_str());
}

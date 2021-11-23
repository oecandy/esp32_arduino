void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(topic);
}

void wifiMqttInit() {
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(maxMqttBufferSize);
}

void mqttPublishMessage(){
  String jsonStr = "{\"device_no\":";
  jsonStr += sensorData.device_no;
  jsonStr += ",\"x_g_val\":";
  jsonStr += sensorData.x_val;
  jsonStr += ",\"y_g_val\":";
  jsonStr += sensorData.y_val;
  jsonStr += ",\"z_g_val\":";
  jsonStr += sensorData.z_val;
  jsonStr += "}";

  mqttClient.publish(DATA_TOPIC, jsonStr.c_str());  
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
      mqttClient.publish(PING_TOPIC, "connected");
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

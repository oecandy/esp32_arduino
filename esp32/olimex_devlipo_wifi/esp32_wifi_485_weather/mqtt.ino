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
      // Once connected, publish an announcement...
      client.publish(CONNECT_TOPIC, "connected");
      mqttPublishHealthCheck();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

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


void mqttPublishHealthCheck(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"rssi\":\"%s\",\"serialNo\":\"%s\",\"heap\":%ld}", WiFi.localIP().toString().c_str(), String(WiFi.RSSI()).c_str(), SERIAL_NO, ESP.getFreeHeap());
  client.publish (PING_TOPIC, message);
}

void sendMessage() {
  String msg = "{\"outTemp\":";
  msg += weatherData.temperature;
  msg += ",\"outHumidity\":";
  msg += weatherData.humidity;
  msg += ",\"rain\":";
  msg += weatherData.rain;
  msg += ",\"hourRain\":";
  msg += weatherData.hourRain;
  msg += ",\"dayRain\":";
  msg += weatherData.dayRain;
  msg += ",\"windDir\":";
  msg += weatherData.windDir;
  msg += ",\"windSpeed\":";
  msg += weatherData.windSpeed;
  msg += ",\"radiation\":";
  msg += weatherData.radiation;
  msg += ",\"UV\":";
  msg += weatherData.uv;
  msg += "}";
  
  client.publish (PUB_TOPIC, msg.c_str());
}

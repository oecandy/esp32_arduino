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
      publish_health_check();
      client.subscribe(SUB_TOPIC);
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
    int switchNo = doc["valveNo"];
    int onOff = doc["state"];
    relayStatus[switchNo-1] = onOff;
    controll_valve(switchNo, onOff);
  }
}

void publish_health_check(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"rssi\":\"%s\",\"serialNo\":\"%s\",\"heap\":%ld}", WiFi.localIP().toString().c_str(), String(WiFi.RSSI()).c_str(), SERIAL_NO, ESP.getFreeHeap());
  client.publish (PING_TOPIC, message);
}

void send_message() {
  String msg = "{\"channel\":";
  msg += CHANNEL;
  msg += ",\"flowCount\":";
  msg += flowCount/6;
  msg += ",\"SW01\":{\"state\":";
  msg += relayStatus[0];
  msg += "},\"SW02\":{\"state\":";
  msg += relayStatus[1];
  msg += "},\"SW03\":{\"state\":";
  msg += relayStatus[2];
  msg += "},\"SW04\":{\"state\":";
  msg += relayStatus[3];
  msg += "},\"SW05\":{\"state\":";
  msg += relayStatus[4];
  msg += "},\"SW06\":{\"state\":";
  msg += relayStatus[5];
  msg += "}}";
  
  client.publish (PUB_TOPIC, msg.c_str());
}

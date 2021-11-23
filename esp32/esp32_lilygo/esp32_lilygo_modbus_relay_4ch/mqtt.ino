void mqtt_setup(){
  client.setServer(mqtt_server, 1883);
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
      client.publish(conn_topic, "connected");
      mqttPublishHealthCheck();
      // ... and resubscribe
      client.subscribe(sub_topic);
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
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
    if(root["SW01"] == 1){
      command_status[0] = 1;
      Serial.println("valve1 open");
    } else if (root["SW01"] == 2){
      command_status[0] = 0;
      Serial.println("valve1 close");
    }
    if(root["SW02"] == 1){
      command_status[1] = 1;
      Serial.println("valve2 open");
    } else if (root["SW02"] == 2){
      command_status[1] = 0;
      Serial.println("valve2 close");
    }
    if(root["SW03"] == 1){
      command_status[2] = 1;
      Serial.println("valve3 open");
    } else if (root["SW03"] == 2){
      command_status[2] = 0;
      Serial.println("valve3 close");
    }
    if(root["SW04"] == 1){
      command_status[3] = 1;
      Serial.println("valve4 open");
    } else if (root["SW04"] == 2){
      command_status[3] = 0;
      Serial.println("valve4 close");
    }
  } else {
    Serial.println("parseObject() failed");
  }
  
}

void mqttPublishHealthCheck(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"mac\":\"%s\",\"relayChannel\":%ld,\"serialNo\":\"%s\",\"heap\":%ld}", ETH.localIP().toString().c_str(), ETH.macAddress().c_str(), RELAY_TYPE, SERIAL_NO, ESP.getFreeHeap());
  client.publish (ping_topic, message);
}

void mqtt_publish_data(){
  // 값 튐 방지
  flowCount1 = flowCount1 > 300 ? 0 : flowCount1;
  flowCount2 = flowCount2 > 300 ? 0 : flowCount2;
  flowCount3 = flowCount3 > 300 ? 0 : flowCount3;
  flowCount4 = flowCount4 > 300 ? 0 : flowCount4;
  
  String jsonData = "{\"relayChannel\":";
  jsonData += RELAY_TYPE;
  jsonData += ",\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"valve\":{";
  jsonData += "\"SW01\":{\"state\":";
  jsonData += command_status[0];
  jsonData += ",\"flowCount\":";
  jsonData += flowCount1/6;
  jsonData += "},\"SW02\":{\"state\":";
  jsonData += command_status[1];
  jsonData += ",\"flowCount\":";
  jsonData += flowCount2/6;
  jsonData += "},\"SW03\":{\"state\":";
  jsonData += command_status[2];
  jsonData += ",\"flowCount\":";
  jsonData += flowCount3/6;
  jsonData += "},\"SW04\":{\"state\":";
  jsonData += command_status[3];
  jsonData += ",\"flowCount\":";
  jsonData += flowCount4/6;
  jsonData += "}}}";
  
  client.publish (data_topic, jsonData.c_str());
}

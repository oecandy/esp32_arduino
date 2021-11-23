void mqtt_setup(){
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  client.setBufferSize(MAX_MQTT_BUFFER_SIZE);
  client.subscribe(SUB_TOPIC);
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
      // ... and resubscribe
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
    if((int)doc["SW01"] == 1){
      relay_status[0] = 1;
      Serial.println("valve1 open");
      digitalWrite(RELAY_1_PIN, relay_status[0]);
    } else if ((int)doc["SW01"] == 2){
      relay_status[0] = 0;
      Serial.println("valve1 close");
      digitalWrite(RELAY_1_PIN, relay_status[0]);
    }
    if((int)doc["SW02"] == 1){
      relay_status[1] = 1;
      Serial.println("valve2 open");
      digitalWrite(RELAY_2_PIN, relay_status[1]);
    } else if ((int)doc["SW02"] == 2){
      relay_status[1] = 0;
      Serial.println("valve2 close");
      digitalWrite(RELAY_2_PIN, relay_status[1]);
    }

    if((int)doc["SW03"] == 1){
      relay_status[2] = 1;
      Serial.println("valve3 open");
      digitalWrite(RELAY_3_PIN, relay_status[2]);
    } else if ((int)doc["SW03"] == 2){
      relay_status[2] = 0;
      Serial.println("valve3 close");
      digitalWrite(RELAY_3_PIN, relay_status[2]);
    }

    if((int)doc["SW04"] == 1){
      relay_status[3] = 1;
      Serial.println("valve4 open");
      digitalWrite(RELAY_4_PIN, relay_status[3]);
    } else if ((int)doc["SW04"] == 2){
      relay_status[3] = 0;
      Serial.println("valve4 close");
      digitalWrite(RELAY_4_PIN, relay_status[3]);
    }
  }
}

void mqttPublishHealthCheck(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"relayChannel\":%ld,\"serialNo\":\"%s\",\"heap\":%ld}", ETH.localIP().toString().c_str(), RELAY_TYPE, SERIAL_NO, ESP.getFreeHeap());
  client.publish (PING_TOPIC, message);
}

void mqtt_publish_data(){
  
  String jsonData = "{\"relayChannel\":";
  jsonData += RELAY_TYPE;
  jsonData += ",\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"valve\":{";
  jsonData += "\"SW01\":{\"state\":";
  jsonData += relay_status[0];
  jsonData += ",\"flowCount\":";
  jsonData += 0;
  jsonData += "},\"SW02\":{\"state\":";
  jsonData += relay_status[1];
  jsonData += ",\"flowCount\":";
  jsonData += 0;
    jsonData += "},\"SW03\":{\"state\":";
  jsonData += relay_status[2];
  jsonData += ",\"flowCount\":";
  jsonData += 0;
    jsonData += "},\"SW04\":{\"state\":";
  jsonData += relay_status[3];
  jsonData += ",\"flowCount\":";
  jsonData += 0;
  jsonData += "}}}";
  
  client.publish (PUB_TOPIC, jsonData.c_str());
}

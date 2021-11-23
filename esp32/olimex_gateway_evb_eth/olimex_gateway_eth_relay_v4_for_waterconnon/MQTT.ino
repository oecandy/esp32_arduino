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
    String clientId = "ESPGPIO-";
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
  //Serial.print((char)payload);
  Serial.println(topic);
  DeserializationError error = deserializeJson(doc, (char*) payload);
  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    if(doc["SW01"] == 1){
      command_status[0] = 1;
      Serial.println("switch1 open");
      digitalWrite(RELAY_PUMP, HIGH);
    } else if (doc["SW01"] == 2){
      command_status[0] = 0;
      digitalWrite(RELAY_PUMP, LOW);
      Serial.println("switch1 close");
    }
    if(doc["SW02"] == 1){
      command_status[1] = 1;
      digitalWrite(RELAY_FAN, HIGH);
      Serial.println("switch2 open");
    } else if (doc["SW02"] == 2){
      command_status[1] = 0;
      digitalWrite(RELAY_FAN, LOW);
      Serial.println("switch2 close");
    }
    
    if(doc["SW03"] == 1){
      command_status[2] = 1;
      digitalWrite(RELAY_SWING, HIGH);
      Serial.println("switch3 open");
    } else if (doc["SW03"] == 2){
      command_status[2] = 0;
      digitalWrite(RELAY_SWING, LOW);
      Serial.println("switch3 close");
    }

    if(doc["SW04"] == 1){
      command_status[3] = 1;
      digitalWrite(RELAY_LIFTING, HIGH);
      Serial.println("switch4 open");
    } else if (doc["SW04"] == 2){
      command_status[3] = 0;
      digitalWrite(RELAY_LIFTING, LOW);
      Serial.println("switch4 close");
    }
    if(doc["SW05"] == 1){
      command_status[4] = 1;
      digitalWrite(RELAY_EXTRA1, HIGH);
      Serial.println("switch5 open");
    } else if (doc["SW04"] == 2){
      command_status[4] = 0;
      digitalWrite(RELAY_EXTRA1, LOW);
      Serial.println("switch5 close");
    }
    if(doc["SW06"] == 1){
      command_status[5] = 1;
      digitalWrite(RELAY_EXTRA2, HIGH);
      Serial.println("switch6 open");
    } else if (doc["SW04"] == 2){
      command_status[5] = 0;
      digitalWrite(RELAY_EXTRA2, LOW);
      Serial.println("switch6 close");
    }
  }
}

void mqttPublishHealthCheck(){
  snprintf(message, 200, "{\"ip\":\"%s\",\"rssi\":%ld,\"mac\":\"%s\",\"relayType\":%ld,\"serialNo\":\"%s\",\"heap\":%ld}", WiFi.localIP().toString().c_str(), WiFi.RSSI(), WiFi.macAddress().c_str(), RELAY_TYPE, SERIAL_NO, ESP.getFreeHeap());
  client.publish (ping_topic, message);
}

void mqtt_publish_data(){
  String jsonData = "{\"relayType\":";
  jsonData += RELAY_TYPE;
  jsonData += ",\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"valve\":{";
  jsonData += "\"SW01\":{\"state\":";
  jsonData += command_status[0];
  jsonData += "},\"SW02\":{\"state\":";
  jsonData += command_status[1];
  jsonData += "},\"SW03\":{\"state\":";
  jsonData += command_status[2];
  jsonData += "},\"SW04\":{\"state\":";
  jsonData += command_status[3];
  jsonData += "},\"SW05\":{\"state\":";
  jsonData += command_status[4];
  jsonData += "},\"SW06\":{\"state\":";
  jsonData += command_status[5];
  jsonData += "}}}";
  client.publish (data_topic, jsonData.c_str());
}

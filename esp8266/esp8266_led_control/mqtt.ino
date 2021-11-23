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
      relayStatus[0] = 1;
      Serial.println("led 1 open");
      digitalWrite(LED_1_PIN, relayStatus[0]);
    } else if ((int)doc["SW01"] == 2){
      relayStatus[0] = 0;
      Serial.println("led 1 close");
      digitalWrite(LED_1_PIN, relayStatus[0]);
    } 
    
    if ((int)doc["SW02"] == 1){
      relayStatus[1] = 0;
      Serial.println("led 2 close");
      digitalWrite(LED_2_PIN, relayStatus[1]);
    } else if ((int)doc["SW02"] == 2){
      relayStatus[1] = 0;
      Serial.println("led 2 close");
      digitalWrite(LED_2_PIN, relayStatus[1]);
    } 
    
    if ((int)doc["SW03"] == 1){
      relayStatus[2] = 0;
      Serial.println("led 3 close");
      digitalWrite(LED_3_PIN, relayStatus[2]);
    } else if ((int)doc["SW03"] == 2){
      relayStatus[2] = 0;
      Serial.println("led 3 close");
      digitalWrite(LED_3_PIN, relayStatus[2]);
    } 
    
    if ((int)doc["SW04"] == 1){
      relayStatus[3] = 0;
      Serial.println("led 4 close");
      digitalWrite(LED_4_PIN, relayStatus[3]);
    } else if ((int)doc["SW04"] == 2){
      relayStatus[3] = 0;
      Serial.println("led 4 close");
      digitalWrite(LED_4_PIN, relayStatus[3]);
    }
  }
  
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
      client.publish(MQTT_CONN_TOPIC, "connected");
      // ... and resubscribe
      client.subscribe(MQTT_SUB_TOPIC);
    } else {
      if(reboot_cnt == 5){
        ESP.restart();
      } else {
        reboot_cnt ++;
      }
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void send_payload(){
  
  String msg = "{\"rssi\":";
  msg += String(WiFi.RSSI());
  msg += ",\"ip\":\"";
  msg += WiFi.localIP().toString();
  msg += "\",\"heap\":";
  msg += ESP.getFreeHeap();
  msg += ",\"channel\":";
  msg += CHANNEL;
  msg += ",\"SW01\":{\"state\":";
  msg += relayStatus[0];
  msg += "},\"SW02\":{\"state\":";
  msg += relayStatus[1];
  msg += "},\"SW03\":{\"state\":";
  msg += relayStatus[2];
  msg += "},\"SW04\":{\"state\":";
  msg += relayStatus[3];
  msg += "}}";

  client.publish (MQTT_PUB_TOPIC, msg.c_str());
}

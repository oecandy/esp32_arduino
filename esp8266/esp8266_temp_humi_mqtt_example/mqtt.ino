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
  } else {
    Serial.println("parseObject() failed");
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
  float temp_result = sumTemp/ARR_CNT;
  
  String jsonStr = "{\"temperature\":";
  jsonStr += temp_result;
  jsonStr += "}";

  client.publish (MQTT_PUB_TOPIC, jsonStr.c_str());
  sumTemp = (float) 0;
}

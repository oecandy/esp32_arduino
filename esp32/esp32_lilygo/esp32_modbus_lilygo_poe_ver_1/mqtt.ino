void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
    if (String(topic).equals(String(weather_topic))){
      set_temp_01((float) root["outTemp"]);
      set_humi_01((float) root["outHumidity"]);
      set_dew_point_sensor_01((float) root["dewpoint"]);
      set_rain_01((float) root["rain"]);
      set_rain_02((float) root["rain"]);
      set_sun_01((float) root["radiation"]);
      set_wind_speed_01((float) root["windSpeed"]);
      set_wind_dir_01((float) root["windDir"]);
      
    } else if (String(topic).equals(String(ec_topic))){
      set_ec_01((float) root["ec"]);
      
    } else if (String(topic).equals(String(soil_topic))){
      if(root["moi"] != NULL){
        set_soil_moi_01((float) root["moi"]);
      }
      if(root["tem"] != NULL){
        set_soil_temp_01((float) root["tem"]);
      }
    } else if (String(topic).equals(String(ph_topic))){
      set_ph_01((float) root["pH"]);
    } else if (String(topic).equals(String(pf_topic))){
      set_soil_water_ten_01((float) root["pF"]);
    }
  } else {
    Serial.println("parseObject() failed");
  }
  
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
      // Once connected, publish an announcement...
      mqttClient.publish(conn_topic, "connected");
      // ... and resubscribe
      mqttClient.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void wifiMqttInit() {
  //WiFi.onEvent(WiFiEvent);
  
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
}

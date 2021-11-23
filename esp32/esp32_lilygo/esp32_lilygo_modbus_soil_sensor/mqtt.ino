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

void mqtt_publish_sensor_data(String jsonData){

  String serial = "";
  switch(sensor_cnt){
    case 1:
      serial = SENSOR_SERIAL_1;
      break;
    case 2:
      serial = SENSOR_SERIAL_2;
      break;
    case 3:
      serial = SENSOR_SERIAL_3;
      break;
    case 4:
      serial = SENSOR_SERIAL_4;
      break;
    default:
      break;
  }

  String topic = soil_topic_prefix;
  topic += serial;
  topic += "/data";

  client.publish (topic.c_str(), jsonData.c_str());
}

void mqtt_publish_air_data(){
  String jsonStr = "{\"temp\":";
  jsonStr += air_sensor.temp;
  jsonStr += ",\"humi\":";
  jsonStr += air_sensor.humi;
  jsonStr += "}";
  
  String topic = air_topic_prefix;
  topic += SENSOR_SERIAL_5;
  topic += "/data";

  client.publish (topic.c_str(), jsonStr.c_str());
}

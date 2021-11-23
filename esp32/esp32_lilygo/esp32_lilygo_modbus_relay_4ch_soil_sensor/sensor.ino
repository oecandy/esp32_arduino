void send_sensor_data(){
  String jsonData = get_json_data(sensor_cnt);
  mqtt_publish_sensor_data(jsonData);

  if(sensor_cnt == 4){
    sensor_cnt = 1;
  } else {
    sensor_cnt++;
  }
}

String get_json_data(int sensor_cnt){
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
  
  String jsonData = "{\"serialNo\":\"";

  if(sensor_cnt == 1){
//    Serial.println("sensor 1 sended");
    loop_read_regs_sensor_1();
    jsonData += serial;
    jsonData += "\",\"sensor\":{\"first\":{\"temp\":";
    jsonData += soil_sensor_1_1.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_1_1.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_1_1.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_1_1.salt;
    jsonData += "},\"second\":{\"temp\":";
    jsonData += soil_sensor_1_2.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_1_2.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_1_2.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_1_2.salt;
    jsonData += "},\"third\":{\"temp\":";
    jsonData += soil_sensor_1_3.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_1_3.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_1_3.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_1_3.salt;
    jsonData += "}}}";
//    Serial.println(jsonData);
  } else if(sensor_cnt == 2){
//    Serial.println("sensor 2 sended");
    loop_read_regs_sensor_2();
    jsonData += serial;
    jsonData += "\",\"sensor\":{\"first\":{\"temp\":";
    jsonData += soil_sensor_2_1.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_2_1.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_2_1.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_2_1.salt;
    jsonData += "},\"second\":{\"temp\":";
    jsonData += soil_sensor_2_2.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_2_2.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_2_2.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_2_2.salt;
    jsonData += "},\"third\":{\"temp\":";
    jsonData += soil_sensor_2_3.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_2_3.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_2_3.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_2_3.salt;
    jsonData += "}}}";
//    Serial.println(jsonData);
  } else if(sensor_cnt == 3){
//    Serial.println("sensor 3 sended");
    loop_read_regs_sensor_3();
    jsonData += serial;
    jsonData += "\",\"sensor\":{\"first\":{\"temp\":";
    jsonData += soil_sensor_3_1.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_3_1.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_3_1.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_3_1.salt;
    jsonData += "},\"second\":{\"temp\":";
    jsonData += soil_sensor_3_2.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_3_2.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_3_2.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_3_2.salt;
    jsonData += "},\"third\":{\"temp\":";
    jsonData += soil_sensor_3_3.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_3_3.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_3_3.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_3_3.salt;
    jsonData += "}}}";
//    Serial.println(jsonData);
  } else if(sensor_cnt == 4){
//    Serial.println("sensor 4 sended");
    loop_read_regs_sensor_4();
    jsonData += serial;
    jsonData += "\",\"sensor\":{\"first\":{\"temp\":";
    jsonData += soil_sensor_4_1.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_4_1.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_4_1.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_4_1.salt;
    jsonData += "},\"second\":{\"temp\":";
    jsonData += soil_sensor_4_2.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_4_2.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_4_2.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_4_2.salt;
    jsonData += "},\"third\":{\"temp\":";
    jsonData += soil_sensor_4_3.temp;
    jsonData += ",\"humi\":";
    jsonData += soil_sensor_4_3.humi;
    jsonData += ",\"ec\":";
    jsonData += soil_sensor_4_3.ec;
    jsonData += ",\"salt\":";
    jsonData += soil_sensor_4_3.salt;
    jsonData += "}}}";
//    Serial.println(jsonData);
  }
  
  return jsonData;
}

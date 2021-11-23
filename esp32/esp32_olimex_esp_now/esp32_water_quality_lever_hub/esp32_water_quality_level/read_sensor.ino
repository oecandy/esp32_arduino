void sensor_loop(){
  unsigned long currentMillis = millis();
  
  if (currentMillis - loopMillis >= INTERVAL_TIME) {
    loopMillis = currentMillis;
    get_water_quality_data();
    get_water_level_data();
    sendPayload();
  }
}

void get_water_quality_data(){
  read_water_quality_value();
  
  char hexstring[18];
  snprintf(hexstring, sizeof(hexstring), "%04X%04X",
           reg_wq_vals[1], reg_wq_vals[0]);
           
  long long_val = strtol(hexstring, NULL, 16);
  float water_quality = *(float *)&long_val;
  
  Serial.println(hexstring);
  Serial.println(long_val);
  Serial.println(water_quality);
  
  myData.water_quality = water_quality;
}

void get_water_level_data(){
  read_water_level_value();
  Serial.println(reg_wl_vals[0]);
  myData.water_level = reg_wl_vals[0] > 32768 ? 0 : reg_wl_vals[0];
}

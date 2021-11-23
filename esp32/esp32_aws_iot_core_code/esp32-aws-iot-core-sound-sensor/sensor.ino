void sensorLoop(){
  unsigned long current_millis = millis();
  if (current_millis - loop_millis >= interval_mills) {
    loop_millis = current_millis;
    if(coll_cnt < SENSOR_COLL_CNT){
      if(coll_cnt == 1){
        timeClient.update();
        coll_timestatmp = timeClient.getEpochTime();
      }
      readSoundSensor();
    } else {
      calculateSoundData();
      publishMessage();
      coll_cnt = 0;
      
    }
  }
}

void calculateSoundData(){
  float temp;
  float sum = 0;
  
  for(int i = 0; i < SENSOR_COLL_CNT; i ++){
    for(int j = 0; j < SENSOR_COLL_CNT - 1; j ++){
      
      if(sound_sensor_datas[j] > sound_sensor_datas [j+1]){
        
        temp = sound_sensor_datas[j];
        sound_sensor_datas[j] = sound_sensor_datas[j+1];
        sound_sensor_datas[j+1] = temp;
        
      }
    }
  }
  int l10_index = ((SENSOR_COLL_CNT/10)* 9) - 1;

  for(int k = 0; k < SENSOR_COLL_CNT; k ++){
    Serial.println(sound_sensor_datas[k]);
    sum += sound_sensor_datas[k];
    Serial.println(sum);
  }

  result.leq = sum/SENSOR_COLL_CNT;
  result.l10 = sound_sensor_datas[l10_index];
  result.lmax = sound_sensor_datas[SENSOR_COLL_CNT-1];
  Serial.println(result.leq);
  Serial.println(result.l10);
  Serial.println(result.lmax);
  
}

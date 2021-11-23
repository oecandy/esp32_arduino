void loop_modbus(){
  if (!mb.slave()) {
    mb.readCoil(idx_cnt, 0, coils, 1, cbWrite);
    Serial.print("valve_");
    Serial.println(idx_cnt);
  }
  mb.task();
  yield();
  
  valve_status[idx_cnt] = coils[0];

  if(idx_cnt == 3){
    idx_cnt = 0;
    mqtt_publish_data();
  } else {
    idx_cnt ++;
  }
  delay(100);
}

void loop_write_coil(){
  for(int i = 0; i < RELAY_TYPE; i ++){
    controll_valve(i, command_status[i]); 
  }
}

void modbus_setup(){
  mb.begin(&Serial2,RE_DE);
  mb.master();
}

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
//  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

void loop_read_regs_sensor_1(){
  int modbus_idx = 10;
  
  if(!mb.slave()){
    mb.readHreg(modbus_idx,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_1_1.humi = reg_vals[0];
      soil_sensor_1_1.temp = reg_vals[1];
      soil_sensor_1_1.ec = reg_vals[2];
      soil_sensor_1_1.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+1,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_1_2.humi = reg_vals[0];
      soil_sensor_1_2.temp = reg_vals[1];
      soil_sensor_1_2.ec = reg_vals[2];
      soil_sensor_1_2.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+2,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_1_3.humi = reg_vals[0];
      soil_sensor_1_3.temp = reg_vals[1];
      soil_sensor_1_3.ec = reg_vals[2];
      soil_sensor_1_3.salt = reg_vals[3];
    }
    yield();
  }
}

void loop_read_regs_sensor_2(){
  int modbus_idx = 20;
  
  if(!mb.slave()){
    mb.readHreg(modbus_idx,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_2_1.humi = reg_vals[0];
      soil_sensor_2_1.temp = reg_vals[1];
      soil_sensor_2_1.ec = reg_vals[2];
      soil_sensor_2_1.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+1,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_2_2.humi = reg_vals[0];
      soil_sensor_2_2.temp = reg_vals[1];
      soil_sensor_2_2.ec = reg_vals[2];
      soil_sensor_2_2.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+2,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_2_3.humi = reg_vals[0];
      soil_sensor_2_3.temp = reg_vals[1];
      soil_sensor_2_3.ec = reg_vals[2];
      soil_sensor_2_3.salt = reg_vals[3];
    }
    yield();
  }
}

void loop_read_regs_sensor_3(){
  int modbus_idx = 30;
  
  if(!mb.slave()){
    mb.readHreg(modbus_idx,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_3_1.humi = reg_vals[0];
      soil_sensor_3_1.temp = reg_vals[1];
      soil_sensor_3_1.ec = reg_vals[2];
      soil_sensor_3_1.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+1,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_3_2.humi = reg_vals[0];
      soil_sensor_3_2.temp = reg_vals[1];
      soil_sensor_3_2.ec = reg_vals[2];
      soil_sensor_3_2.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+2,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_3_3.humi = reg_vals[0];
      soil_sensor_3_3.temp = reg_vals[1];
      soil_sensor_3_3.ec = reg_vals[2];
      soil_sensor_3_3.salt = reg_vals[3];
    }
    yield();
  }
}

void loop_read_regs_sensor_4(){
  int modbus_idx = 40;
  
  if(!mb.slave()){
    mb.readHreg(modbus_idx,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_4_1.humi = reg_vals[0];
      soil_sensor_4_1.temp = reg_vals[1];
      soil_sensor_4_1.ec = reg_vals[2];
      soil_sensor_4_1.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+1,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_4_2.humi = reg_vals[0];
      soil_sensor_4_2.temp = reg_vals[1];
      soil_sensor_4_2.ec = reg_vals[2];
      soil_sensor_4_2.salt = reg_vals[3];
    }
    yield();
  }

  delay(100);

  if(!mb.slave()){
    mb.readHreg(modbus_idx+2,0,reg_vals,10);
    while(mb.slave()){
      mb.task();
      delay(10);
      soil_sensor_4_3.humi = reg_vals[0];
      soil_sensor_4_3.temp = reg_vals[1];
      soil_sensor_4_3.ec = reg_vals[2];
      soil_sensor_4_3.salt = reg_vals[3];
    }
    yield();
  }
}

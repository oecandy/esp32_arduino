void sendMessage() {
  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"device_no\":";
  msg += SENSOR_NO;
  msg += ",\"state\":"; // 1 : 화재 발생, 0 : 정상
  msg += registerDatas[6];
  msg += ",\"temperature\":";
  msg += (float)registerDatas[0]/10;
  msg += ",\"humidity\":";
  msg += (float)registerDatas[1]/10;
  msg += ",\"serialNo\":\"";
  msg += SERIAL_NO;
  msg += "\"}}";
  mesh.sendSingle(ROOT_ID, msg);
  Serial.println(msg);
}

void mesh_setup(){
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.setContainsRoot(true);
}

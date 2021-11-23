void loopSensor(){
  val = hallRead();
//  Serial.print("Hall sensor measurement: ");
//  Serial.println(val);
  if(val > THRES_VAL){
    locked = 0; //닫힘
  } else {
    locked = 1; //열림, 문이 열린 상태로 있어도 걸쇠는 Normally closed, 향후 밴드 시나리오에서 밴드착용자 근처가면 open
  }
  //ESPNowSend locked상태 마스터에서 MQTT retained로 유지
}

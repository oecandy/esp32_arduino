void setModbusMetadata(){
  // 기관 정보 및 센서 정보 메타 데이터
  modbus_array[0]= 0;
  mb.Hreg(0, modbus_array[0]);
  modbus_array[1]= 0;
  mb.Hreg(1, modbus_array[1]);
  modbus_array[2]= product_type;
  mb.Hreg(2, modbus_array[2]);
  modbus_array[3]= 0;
  mb.Hreg(3, modbus_array[3]);
  modbus_array[4]= protocol_version;
  mb.Hreg(4, modbus_array[4]);
  modbus_array[5]= device_count;
  mb.Hreg(5, modbus_array[5]);

  //센서 분류 메타데이터 Addr : 101~ 140 까지 132부터는 예비 이므로 작성하지 않음
  modbus_array[100]= 1; // 온도 1
  mb.Hreg(100, modbus_array[100]);
  modbus_array[101]= 1; // 온도 2
  mb.Hreg(101, modbus_array[101]);
  modbus_array[102]= 1; // 온도 3
  mb.Hreg(102, modbus_array[102]);
  modbus_array[103]= 2; // 습도 1
  mb.Hreg(103, modbus_array[103]);
  modbus_array[104]= 3; // 이슬점 센서
  mb.Hreg(104, modbus_array[104]);
  modbus_array[105]= 4; // 감우 센서
  mb.Hreg(105, modbus_array[105]);
  modbus_array[106]= 5; // 유량 센서
  mb.Hreg(106, modbus_array[106]);
  modbus_array[107]= 6; // 강우 센서
  mb.Hreg(107, modbus_array[107]);
  modbus_array[108]= 7; // 일사 센서
  mb.Hreg(108, modbus_array[108]);
  modbus_array[109]= 8; // 풍속 센서
  mb.Hreg(109, modbus_array[109]);
  modbus_array[110]= 9; // 풍향 센서
  mb.Hreg(110, modbus_array[110]);
  modbus_array[111]= 10; // 전압 센서
  mb.Hreg(111, modbus_array[111]);
  modbus_array[112]= 11; // CO2 센서
  mb.Hreg(112, modbus_array[112]);
  modbus_array[113]= 12; // EC 센서
  mb.Hreg(113, modbus_array[113]);
  modbus_array[114]= 13; // 광자양 센서
  mb.Hreg(114, modbus_array[114]);
  modbus_array[115]= 14; // 토양 함수율 센서
  mb.Hreg(115, modbus_array[115]);
  modbus_array[116]= 15; // 토양 수분 장력 센서
  mb.Hreg(116, modbus_array[116]);
  modbus_array[117]= 16; // PH 센서
  mb.Hreg(117, modbus_array[117]);
  modbus_array[118]= 17; // 지온 센서
  mb.Hreg(118, modbus_array[118]);
  modbus_array[119]= 1; // 온도4 센서
  mb.Hreg(119, modbus_array[119]);
  modbus_array[120]= 1; // 온도5 센서
  mb.Hreg(120, modbus_array[120]);
  modbus_array[121]= 1; // 온도6 센서
  mb.Hreg(121, modbus_array[121]);
  modbus_array[122]= 1; // 온도7 센서
  mb.Hreg(122, modbus_array[122]);
  modbus_array[123]= 1; // 온도8 센서
  mb.Hreg(123, modbus_array[123]);
  modbus_array[124]= 1; // 온도9 센서
  mb.Hreg(124, modbus_array[124]);
  modbus_array[125]= 1; // 온도10 센서
  mb.Hreg(125, modbus_array[125]);
  modbus_array[126]= 2; // 습도2 센서
  mb.Hreg(126, modbus_array[126]);
  modbus_array[127]= 2; // 습도3 센서
  mb.Hreg(127, modbus_array[127]);
  modbus_array[128]= 19; // 무게1 센서
  mb.Hreg(128, modbus_array[128]);
  modbus_array[129]= 19; // 무게2 센서
  mb.Hreg(129, modbus_array[129]);

}

void setMqttMetadata(){
  mqtt_array[0]= 0;
  mqtt_array[1]= 0;
  mqtt_array[2]= product_type;
  mqtt_array[3]= 0;
  mqtt_array[4]= protocol_version;
  mqtt_array[5]= device_count;

    //센서 분류 메타데이터 Addr : 101~ 140 까지 131부터는 예비 이므로 작성하지 않음
  mqtt_array[100]= 1; // 온도 1
  mqtt_array[101]= 1; // 온도 2
  mqtt_array[102]= 1; // 온도 3
  mqtt_array[103]= 2; // 습도 1
  mqtt_array[104]= 3; // 이슬점 센서
  mqtt_array[105]= 4; // 감우 센서
  mqtt_array[106]= 5; // 유량 센서
  mqtt_array[107]= 6; // 강우 센서
  mqtt_array[108]= 7; // 일사 센서
  mqtt_array[109]= 8; // 풍속 센서
  mqtt_array[110]= 9; // 풍향 센서
  mqtt_array[111]= 10; // 전압 센서
  mqtt_array[112]= 11; // CO2 센서
  mqtt_array[113]= 12; // EC 센서
  mqtt_array[114]= 13; // 광자양 센서
  mqtt_array[115]= 14; // 토양 함수율 센서
  mqtt_array[116]= 15; // 토양 수분 장력 센서
  mqtt_array[117]= 17; // PH 센서
  mqtt_array[118]= 18; // 지온 센서
  mqtt_array[119]= 1; // 온도4 센서
  mqtt_array[120]= 1; // 온도5 센서
  mqtt_array[121]= 1; // 온도6 센서
  mqtt_array[122]= 1; // 온도7 센서
  mqtt_array[123]= 1; // 온도8 센서
  mqtt_array[124]= 1; // 온도9 센서
  mqtt_array[125]= 1; // 온도10 센서
  mqtt_array[126]= 2; // 습도2 센서
  mqtt_array[127]= 2; // 습도3 센서
  mqtt_array[128]= 19; // 무게1 센서
  mqtt_array[129]= 19; // 무게2 센서
}


void setModbusAddress(){
  for(int i=0; i < register_size; i ++){
    mb.addHreg(i);
    mb.Hreg(i, 0);
  }
}

void loop_modbus(){
  mb.task();
  yield();
  delay(1000);
}

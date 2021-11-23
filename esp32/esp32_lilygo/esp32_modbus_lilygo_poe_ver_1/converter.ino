void convert_byte(float x){
  
  int *pointer;
  pointer = (int*)&x;
  
  int n = *pointer;
  String num_str = String(n,HEX);
  String n_str = num_str.substring(0,4);
  String p_str = num_str.substring(4);
  char n_ch[] = {};
  char p_ch[] = {};
  
  n_str.toCharArray(n_ch, 5, 0);
  data_convert_array[1] = (uint16_t) strtol(n_ch, 0, 16);
  
  p_str.toCharArray(p_ch, 5, 0);
  data_convert_array[0] = (uint16_t) strtol(p_ch, 0, 16);
}

void set_temp_01(float temp){
  convert_byte(temp);
  modbus_array[204] = 0;
  mb.Hreg(204, modbus_array[204]);
  modbus_array[202] = data_convert_array[0];
  mb.Hreg(202, modbus_array[202]);
  modbus_array[203] = data_convert_array[1];
  mb.Hreg(203, modbus_array[203]);

  mqtt_array[204] = 0;
  mqtt_array[202] = data_convert_array[0];
  mqtt_array[203] = data_convert_array[1];
}

void set_temp_02(float temp){
  convert_byte(temp);
  modbus_array[207] = 0;
  mb.Hreg(207, modbus_array[207]);
  modbus_array[205] = data_convert_array[0];
  mb.Hreg(205, modbus_array[205]);
  modbus_array[206] = data_convert_array[1];
  mb.Hreg(206, modbus_array[206]);

  mqtt_array[207] = 0;
  mqtt_array[205] = data_convert_array[0];
  mqtt_array[206] = data_convert_array[1];
}

void set_temp_03(float temp){
  convert_byte(temp);
  modbus_array[210] = 0;
  mb.Hreg(210, modbus_array[210]);
  modbus_array[208] = data_convert_array[0];
  mb.Hreg(208, modbus_array[208]);
  modbus_array[209] = data_convert_array[1];
  mb.Hreg(209, modbus_array[209]);

  mqtt_array[210] = 0;
  mqtt_array[208] = data_convert_array[0];
  mqtt_array[209] = data_convert_array[1];
}

void set_temp_04(float temp){
  convert_byte(temp);
  modbus_array[261] = 0;
  mb.Hreg(261, modbus_array[261]);
  modbus_array[259] = data_convert_array[0];
  mb.Hreg(259, modbus_array[259]);
  modbus_array[260] = data_convert_array[1];
  mb.Hreg(260, modbus_array[260]);

  mqtt_array[261] = 0;
  mqtt_array[259] = data_convert_array[0];
  mqtt_array[260] = data_convert_array[1];
}

void set_temp_05(float temp){
  convert_byte(temp);
  modbus_array[264] = 0;
  mb.Hreg(264, modbus_array[264]);
  modbus_array[262] = data_convert_array[0];
  mb.Hreg(262, modbus_array[262]);
  modbus_array[263] = data_convert_array[1];
  mb.Hreg(263, modbus_array[263]);

  mqtt_array[264] = 0;
  mqtt_array[262] = data_convert_array[0];
  mqtt_array[263] = data_convert_array[1];
}

void set_temp_06(float temp){
  convert_byte(temp);
  modbus_array[267] = 0;
  mb.Hreg(267, modbus_array[267]);
  modbus_array[265] = data_convert_array[0];
  mb.Hreg(265, modbus_array[265]);
  modbus_array[266] = data_convert_array[1];
  mb.Hreg(266, modbus_array[266]);

  mqtt_array[267] = 0;
  mqtt_array[265] = data_convert_array[0];
  mqtt_array[266] = data_convert_array[1];
}

void set_temp_07(float temp){
  convert_byte(temp);
  modbus_array[270] = 0;
  mb.Hreg(270, modbus_array[270]);
  modbus_array[268] = data_convert_array[0];
  mb.Hreg(268, modbus_array[268]);
  modbus_array[269] = data_convert_array[1];
  mb.Hreg(269, modbus_array[269]);

  mqtt_array[270] = 0;
  mqtt_array[268] = data_convert_array[0];
  mqtt_array[269] = data_convert_array[1];
}

void set_temp_08(float temp){
  convert_byte(temp);
  modbus_array[273] = 0;
  mb.Hreg(273, modbus_array[273]);
  modbus_array[271] = data_convert_array[0];
  mb.Hreg(271, modbus_array[271]);
  modbus_array[272] = data_convert_array[1];
  mb.Hreg(272, modbus_array[272]);

  mqtt_array[273] = 0;
  mqtt_array[271] = data_convert_array[0];
  mqtt_array[272] = data_convert_array[1];
}

void set_temp_09(float temp){
  convert_byte(temp);
  modbus_array[276] = 0;
  mb.Hreg(276, modbus_array[276]);
  modbus_array[274] = data_convert_array[0];
  mb.Hreg(274, modbus_array[274]);
  modbus_array[275] = data_convert_array[1];
  mb.Hreg(275, modbus_array[275]);

  mqtt_array[276] = 0;
  mqtt_array[274] = data_convert_array[0];
  mqtt_array[275] = data_convert_array[1];
}

void set_temp_10(float temp){
  convert_byte(temp);
  modbus_array[279] = 0;
  mb.Hreg(279, modbus_array[279]);
  modbus_array[277] = data_convert_array[0];
  mb.Hreg(277, modbus_array[277]);
  modbus_array[278] = data_convert_array[1];
  mb.Hreg(278, modbus_array[278]);

  mqtt_array[279] = 0;
  mqtt_array[277] = data_convert_array[0];
  mqtt_array[278] = data_convert_array[1];
}

void set_humi_01(float humi){
  convert_byte(humi);
  modbus_array[213] = 0;
  mb.Hreg(213, modbus_array[213]);
  modbus_array[211] = data_convert_array[0];
  mb.Hreg(211, modbus_array[211]);
  modbus_array[212] = data_convert_array[1];
  mb.Hreg(212, modbus_array[212]);

  mqtt_array[213] = 0;
  mqtt_array[211] = data_convert_array[0];
  mqtt_array[212] = data_convert_array[1];
}

void set_humi_02(float humi){
  convert_byte(humi);
  modbus_array[282] = 0;
  mb.Hreg(282, modbus_array[282]);
  modbus_array[280] = data_convert_array[0];
  mb.Hreg(280, modbus_array[280]);
  modbus_array[281] = data_convert_array[1];
  mb.Hreg(281, modbus_array[281]);

  mqtt_array[282] = 0;
  mqtt_array[280] = data_convert_array[0];
  mqtt_array[281] = data_convert_array[1];
}

void set_humi_03(float humi){
  convert_byte(humi);
  modbus_array[285] = 0;
  mb.Hreg(285, modbus_array[285]);
  modbus_array[283] = data_convert_array[0];
  mb.Hreg(283, modbus_array[283]);
  modbus_array[284] = data_convert_array[1];
  mb.Hreg(284, modbus_array[284]);

  mqtt_array[285] = 0;
  mqtt_array[283] = data_convert_array[0];
  mqtt_array[284] = data_convert_array[1];
}

void set_dew_point_sensor_01(float dew_point){
  convert_byte(dew_point);
  modbus_array[216] = 0;
  mb.Hreg(216, modbus_array[216]);
  modbus_array[214] = data_convert_array[0];
  mb.Hreg(214, modbus_array[214]);
  modbus_array[215] = data_convert_array[1];
  mb.Hreg(215, modbus_array[215]);

  mqtt_array[216] = 0;
  mqtt_array[214] = data_convert_array[0];
  mqtt_array[215] = data_convert_array[1];
}

void set_rain_01(float rain){
  convert_byte(rain);
  modbus_array[219] = 0;
  mb.Hreg(219, modbus_array[219]);
  modbus_array[217] = data_convert_array[0];
  mb.Hreg(217, modbus_array[217]);
  modbus_array[218] = data_convert_array[1];
  mb.Hreg(218, modbus_array[218]);

  mqtt_array[219] = 0;
  mqtt_array[217] = data_convert_array[0];
  mqtt_array[218] = data_convert_array[1];
}

void set_flow_01(float flow){
  convert_byte(flow);
  modbus_array[222] = 0;
  mb.Hreg(222, modbus_array[222]);
  modbus_array[220] = data_convert_array[0];
  mb.Hreg(220, modbus_array[220]);
  modbus_array[221] = data_convert_array[1];
  mb.Hreg(221, modbus_array[221]);

  mqtt_array[222] = 0;
  mqtt_array[220] = data_convert_array[0];
  mqtt_array[221] = data_convert_array[1];
}

void set_rain_02(float rain){
  convert_byte(rain);
  modbus_array[225] = 0;
  mb.Hreg(225, modbus_array[225]);
  modbus_array[223] = data_convert_array[0];
  mb.Hreg(223, modbus_array[223]);
  modbus_array[224] = data_convert_array[1];
  mb.Hreg(224, modbus_array[224]);

  mqtt_array[225] = 0;
  mqtt_array[223] = data_convert_array[0];
  mqtt_array[224] = data_convert_array[1];
}

void set_sun_01(float sun){
  convert_byte(sun);
  modbus_array[228] = 0;
  mb.Hreg(228, modbus_array[228]);
  modbus_array[226] = data_convert_array[0];
  mb.Hreg(226, modbus_array[226]);
  modbus_array[227] = data_convert_array[1];
  mb.Hreg(227, modbus_array[227]);

  mqtt_array[228] = 0;
  mqtt_array[226] = data_convert_array[0];
  mqtt_array[227] = data_convert_array[1];
}

void set_wind_speed_01(float wind_speed){
  convert_byte(wind_speed);
  modbus_array[231] = 0;
  mb.Hreg(231, modbus_array[231]);
  modbus_array[229] = data_convert_array[0];
  mb.Hreg(229, modbus_array[229]);
  modbus_array[230] = data_convert_array[1];
  mb.Hreg(230, modbus_array[230]);

  mqtt_array[231] = 0;
  mqtt_array[229] = data_convert_array[0];
  mqtt_array[230] = data_convert_array[1];
}

void set_wind_dir_01(float wind_dir){
  convert_byte(wind_dir);
  modbus_array[234] = 0;
  mb.Hreg(234, modbus_array[234]);
  modbus_array[232] = data_convert_array[0];
  mb.Hreg(232, modbus_array[232]);
  modbus_array[233] = data_convert_array[1];
  mb.Hreg(233, modbus_array[233]);

  mqtt_array[234] = 0;
  mqtt_array[232] = data_convert_array[0];
  mqtt_array[233] = data_convert_array[1];
}

void set_vol_01(float vol){
  convert_byte(vol);
  modbus_array[237] = 0;
  mb.Hreg(237, modbus_array[237]);
  modbus_array[235] = data_convert_array[0];
  mb.Hreg(235, modbus_array[235]);
  modbus_array[236] = data_convert_array[1];
  mb.Hreg(236, modbus_array[236]);

  mqtt_array[237] = 0;
  mqtt_array[235] = data_convert_array[0];
  mqtt_array[236] = data_convert_array[1];
}

void set_co2_01(float co2){
  convert_byte(co2);
  modbus_array[240] = 0;
  mb.Hreg(240, modbus_array[240]);
  modbus_array[238] = data_convert_array[0];
  mb.Hreg(238, modbus_array[238]);
  modbus_array[239] = data_convert_array[1];
  mb.Hreg(239, modbus_array[239]);

  mqtt_array[240] = 0;
  mqtt_array[238] = data_convert_array[0];
  mqtt_array[239] = data_convert_array[1];
}

void set_ec_01(float ec){
  convert_byte(ec);
  modbus_array[243] = 0;
  mb.Hreg(243, modbus_array[243]);
  modbus_array[241] = data_convert_array[0];
  mb.Hreg(241, modbus_array[241]);
  modbus_array[242] = data_convert_array[1];
  mb.Hreg(242, modbus_array[242]);

  mqtt_array[243] = 0;
  mqtt_array[241] = data_convert_array[0];
  mqtt_array[242] = data_convert_array[1];
}

void set_photon_01(float photon){
  convert_byte(photon);
  modbus_array[246] = 0;
  mb.Hreg(246, modbus_array[246]);
  modbus_array[244] = data_convert_array[0];
  mb.Hreg(244, modbus_array[244]);
  modbus_array[245] = data_convert_array[1];
  mb.Hreg(245, modbus_array[245]);

  mqtt_array[246] = 0;
  mqtt_array[244] = data_convert_array[0];
  mqtt_array[245] = data_convert_array[1];
}

void set_soil_moi_01(float soil_moi){
  convert_byte(soil_moi);
  modbus_array[249] = 0;
  mb.Hreg(249, modbus_array[249]);
  modbus_array[247] = data_convert_array[0];
  mb.Hreg(247, modbus_array[247]);
  modbus_array[248] = data_convert_array[1];
  mb.Hreg(248, modbus_array[248]);

  mqtt_array[249] = 0;
  mqtt_array[247] = data_convert_array[0];
  mqtt_array[248] = data_convert_array[1];
}

void set_soil_water_ten_01(float soil_water_ten){
  convert_byte(soil_water_ten);
  modbus_array[252] = 0;
  mb.Hreg(252, modbus_array[252]);
  modbus_array[250] = data_convert_array[0];
  mb.Hreg(250, modbus_array[250]);
  modbus_array[251] = data_convert_array[1];
  mb.Hreg(251, modbus_array[251]);

  mqtt_array[252] = 0;
  mqtt_array[250] = data_convert_array[0];
  mqtt_array[251] = data_convert_array[1];
}

void set_ph_01(float ph){
  convert_byte(ph);
  modbus_array[255] = 0;
  mb.Hreg(255, modbus_array[255]);
  modbus_array[253] = data_convert_array[0];
  mb.Hreg(253, modbus_array[253]);
  modbus_array[254] = data_convert_array[1];
  mb.Hreg(254, modbus_array[254]);

  mqtt_array[255] = 0;
  mqtt_array[253] = data_convert_array[0];
  mqtt_array[254] = data_convert_array[1];
}

void set_soil_temp_01(float soil_temp){
  convert_byte(soil_temp);
  modbus_array[258] = 0;
  mb.Hreg(258, modbus_array[258]);
  modbus_array[256] = data_convert_array[0];
  mb.Hreg(256, modbus_array[256]);
  modbus_array[257] = data_convert_array[1];
  mb.Hreg(257, modbus_array[257]);

  mqtt_array[258] = 0;
  mqtt_array[256] = data_convert_array[0];
  mqtt_array[257] = data_convert_array[1];
}


void set_weight_01(float weight){
  convert_byte(weight);
  modbus_array[288] = 0;
  mb.Hreg(288, modbus_array[288]);
  modbus_array[286] = data_convert_array[0];
  mb.Hreg(286, modbus_array[286]);
  modbus_array[287] = data_convert_array[1];
  mb.Hreg(287, modbus_array[287]);

  mqtt_array[288] = 0;
  mqtt_array[286] = data_convert_array[0];
  mqtt_array[287] = data_convert_array[1];
}

void set_weight_02(float weight){
  convert_byte(weight);
  modbus_array[291] = 0;
  mb.Hreg(291, modbus_array[291]);
  modbus_array[289] = data_convert_array[0];
  mb.Hreg(289, modbus_array[289]);
  modbus_array[290] = data_convert_array[1];
  mb.Hreg(290, modbus_array[290]);

  mqtt_array[291] = 0;
  mqtt_array[289] = data_convert_array[0];
  mqtt_array[290] = data_convert_array[1];
}

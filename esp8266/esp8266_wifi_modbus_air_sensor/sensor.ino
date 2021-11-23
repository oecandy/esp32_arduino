void send_sensor_data(){
  loop_read_regs_air_sensor();
  mqtt_publish_air_data();
}

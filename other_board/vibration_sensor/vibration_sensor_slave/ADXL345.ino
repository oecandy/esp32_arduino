// 2functions arrInit() and readAcc()
void arrInit (){
    if (!myAcc.init()) {
        Serial.println("ADXL345 not connected!");
        ESP.restart();
    }

/* Choose the data rate         Hz
    ADXL345_DATA_RATE_3200    3200
    ADXL345_DATA_RATE_1600    1600
    ADXL345_DATA_RATE_800      800
    ADXL345_DATA_RATE_400      400
    ADXL345_DATA_RATE_200      200
    ADXL345_DATA_RATE_100      100
    ADXL345_DATA_RATE_50        50 <-- selected
    ADXL345_DATA_RATE_25        25
    ADXL345_DATA_RATE_12_5      12.5  
    ADXL345_DATA_RATE_6_25       6.25
    ADXL345_DATA_RATE_3_13       3.13
    ADXL345_DATA_RATE_1_56       1.56
    ADXL345_DATA_RATE_0_78       0.78
    ADXL345_DATA_RATE_0_39       0.39
    ADXL345_DATA_RATE_0_20       0.20
    ADXL345_DATA_RATE_0_10       0.10
*/
    myAcc.setDataRate(ADXL345_DATA_RATE_50);
    Serial.print("Data rate: ");
    Serial.println(myAcc.getDataRateAsString());

/* In full resolution raw values the size of the raw values depend 
    on the range: 2g = 10 bit; 4g = 11 bit; 8g = 12 bit; 16g =13 bit;
    uncomment to change to 10 bit for all ranges. 
 */
    //myAcc.setFullRes(false);

/* Choose the measurement range
    ADXL345_RANGE_16G    16g  <-- selected
    ADXL345_RANGE_8G      8g     
    ADXL345_RANGE_4G      4g   
    ADXL345_RANGE_2G      2g
*/
    switch(GAIN_VAL){
      case 2:
        myAcc.setRange(ADXL345_RANGE_2G);
        break;
      case 4:
        myAcc.setRange(ADXL345_RANGE_4G);
        break;
      case 8:
        myAcc.setRange(ADXL345_RANGE_8G);
        break;
      case 16:
        myAcc.setRange(ADXL345_RANGE_16G);
        break;
      default:
        myAcc.setRange(ADXL345_RANGE_16G);
        break;
    };
    Serial.print("  /  g-Range: ");
    Serial.println(myAcc.getRangeAsString());
    Serial.println();

}

void readAcc (){
    for (int i = 0; i < COLLECT_COUNT; i++){         //after this xyz data is completed in float array
      xyzFloat raw = myAcc.getGValues();
      x[i] = { raw.x };
      y[i] = { raw.y };
      z[i] = { raw.z }; 
    }
}

float getMaxValue(float* n, int16_t size) {
  float max = n[0];

  for (int i = 1; i < size; i++)
    if (n[i] > max) max = n[i];

  return max;
}

float getMinValue(int16_t* n, int16_t size) {
  float min = n[0];

  for (int i = 1; i < size; i++)
    if (n[i] < min) min = n[i];

  return min;
}

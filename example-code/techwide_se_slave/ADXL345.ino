// 2functions arrInit() and readAcc()
void arrInit (){
    if (!myAcc.init()) {
        Serial.println("ADXL345 not connected!");
    }

/* Choose the data rate         Hz
    ADXL345_DATA_RATE_3200    3200
    ADXL345_DATA_RATE_1600    1600
    ADXL345_DATA_RATE_800      800
    ADXL345_DATA_RATE_400      400
    ADXL345_DATA_RATE_200      200
    ADXL345_DATA_RATE_100      100
    ADXL345_DATA_RATE_50        50
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
    myAcc.setDataRate(ADXL345_DATA_RATE_3200);
    Serial.print("Data rate: ");
    Serial.println(myAcc.getDataRateAsString());

/* In full resolution raw values the size of the raw values depend 
    on the range: 2g = 10 bit; 4g = 11 bit; 8g = 12 bit; 16g =13 bit;
    uncomment to change to 10 bit for all ranges. 
 */
    //myAcc.setFullRes(false);

/* Choose the measurement range
    ADXL345_RANGE_16G    16g     
    ADXL345_RANGE_8G      8g     
    ADXL345_RANGE_4G      4g   
    ADXL345_RANGE_2G      2g
*/
    switch(gain){
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

/* Uncomment to enable Low Power Mode. It saves power but slightly
    increases noise. LowPower only affetcs Data Rates 12.5 Hz to 400 Hz.
*/
    // myAcc.setLowPower(true);
}

void readAcc (){
    for (int i = 0; i < 2048; i++){         //after this xyz data is completed in float array
    xyzFloat raw = myAcc.getRawValues();
    x[i] = { raw.x };
    y[i] = { raw.y };
    z[i] = { raw.z };
}
    axis[0] = (int16_t) 1;
    axis[1] = (int16_t) getTemperature(); // temperature data;
    axis[2] = (int16_t) getVoltage(); // battery data;
    axis[3] = (int16_t) bootCount;
    Serial.println("x axis : ");
    sendAxisInfo();
    
    for (int j = 0; j < 64; j++){              // Split x data[2048] to payload[250], send to GW and reset payload reapeat 7times (till 2000)
    memcpy(payload, x + j * 32, 128);
    sendPayload();
    payload[32] = {};
}
    delay(1500);
    
    axis[0] = 3;
    Serial.println("y axis : ");
    sendAxisInfo();
    
    for (int k = 0; k < 64; k++){              // Split y data
    memcpy(payload, y + k * 32, 128);
    sendPayload();
    payload[32] = {};
}
    delay(1500);

    axis[0] = 5;
    Serial.println("z axis : ");
    sendAxisInfo();
    
    for (int l = 0; l < 64; l++){              // Split z data[2048] to payload[250], send to GW and reset payload reapeat 7times (till 2000)
    memcpy(payload, z + l * 32, 128);
    sendPayload();
    payload[32] = {};
}

    Serial.println();
    printf("Count: %d finished..., \n\n", bootCount);
}

/*
    Serial.println(8);
    printf("%0.2f, ", payload[0]);
    printf("%0.2f, \n", x[2000]);
    printf("%0.2f, ", payload[1]);
    printf("%0.2f, \n", x[2001]);
    printf("%0.2f, ", payload[2]);
    printf("%0.2f, \n", x[2002]);
    printf("%0.2f, ", payload[16]);
    printf("%0.2f, \n", x[2006]);
    printf("%0.2f, ", payload[24]);
    printf("%0.2f, \n", x[2024]);
    printf("%0.2f, ", payload[47]);
    printf("%0.2f, \n", x[2047]);
    printf("%0.2f, ", payload[48]);
    printf("%0.2f, \n", x[2048]);

    int piece_length = 249;
    memcpy(x1, x, piece_length);
    x1[piece_length] = '\0';
    memcpy(x2, x+1*piece_length, piece_length);
    x2[piece_length] = '\0';
    memcpy(x3, x+2*piece_length, piece_length);
    x3[piece_length] = '\0';
*/

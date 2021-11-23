// AP mode에서 slave들을 scan하기
// 게이트웨이 당 노드 수는 max 16, ESPNOW max 20 OK,
// 현재 데이터셋 1개 전달 시간이 3분 최대한 줄여보고, 
// 최소 전송 주기를 10분이라고 했을 때, 노드 수 계산
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  //reset slaves
  memset(slaves, 0, sizeof(slaves));
  SlaveCnt = 0;
  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found 스캔된 ESPNOW장비 나열
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);   // [밴드형 slave:slave쪽에서 작동하는 지 확인할 것]
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
      }
      delay(10);
      // Check if the current device starts with `Slave` 슬레이브 SSID는 Slave로 시작함 slave코드 참조
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest 
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // Get BSSID => Mac Address of the Slave 맥어드레스 이용 BSSID 만들기
        int mac[6];

        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slaves[SlaveCnt].peer_addr[ii] = (uint8_t) mac[ii];
          }
        }
        slaves[SlaveCnt].channel = CHANNEL; // pick a channel 슬레이브 채널 지정
        slaves[SlaveCnt].encrypt = 0; // no encryption 암호화 유1무0  
        SlaveCnt++;
      }
    }
  }

  if (SlaveCnt > 0) {
    Serial.print(SlaveCnt); Serial.println(" Slave(s) found, processing..");
  } else {
    Serial.println("No Slave Found, trying again.");
  }

  // clean up ram 스캔 기록 삭제
  
}

// Slave가 Master와 pair되어 있나 check하고, 아니면 Master와 pair한다.
void manageSlave(int slaveIdx) {
  if (SlaveCnt > 0) {
      Serial.print("Processing: ");
      for (int ii = 0; ii < 6; ++ii ) {
        Serial.print((uint8_t) slaves[slaveIdx].peer_addr[ii], HEX);
        if (ii != 5) Serial.print(":");
      }
      Serial.print(" Status: ");
      // check if the peer exists 피어 존재 여부 확인
      bool exists = esp_now_is_peer_exist(slaves[slaveIdx].peer_addr);
      if (exists) {
        // Slave already paired. 이미 페어되어 있으면
        Serial.println("Already Paired");
      } else {
        // Slave not paired, attempt pair 페어 안되어 있으면, 시도
        esp_err_t addStatus = esp_now_add_peer(&slaves[slaveIdx]);
        if (addStatus == ESP_OK) {
          // Pair success 페어링 성공
          Serial.println("Pair success");
        } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
          // 이하 Errors
          Serial.println("ESPNOW Not Init");
        } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
          Serial.println("Add Peer - Invalid Argument");
        } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
          Serial.println("Peer list full");
        } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
          Serial.println("Out of memory");
        } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
          Serial.println("Peer Exists");
        } else {
          Serial.println("Not sure what happened");
        }
        delay(100);
      }
  } else {
    // No slave found to process 검색된 슬레이브 없음
    Serial.println("No Slave found to process");
  }
}

// send data (espNow)
void sendData(int slaveIdx) {
    accessInfo[0] = 1;
    accessInfo[1] = sleepTime;
    accessInfo[2] = gain;
    const uint8_t *peer_addr = slaves[slaveIdx].peer_addr;
    esp_err_t result = esp_now_send(peer_addr, (uint8_t *) &accessInfo, sizeof(accessInfo));
    Serial.print("Send Status: ");
    if (result == ESP_OK) {
      Serial.println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
      // How did we get so far!!
      Serial.println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
      Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
      Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
      Serial.println("Peer not found.");
    } else {
      Serial.println("Not sure what happened");
    }
    targetRssi = WiFi.RSSI(slaveIdx);
}

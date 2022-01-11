//wifi connection
void connectWifi() {
  while (1) {
    //Wifi 초기화
    Serial.println(F("\r\n---------- AT+RST ----------\r\n"));
    WIFISerial.println(F("AT+RST"));
    responseSerial("IP");

    //모드 설정(1로 설정)
    Serial.println(F("\r\n---------- AT+CWMODE ----------\r\n"));
    WIFISerial.println(F("AT+CWMODE=1"));
    responseSerial("OK");

    while (1) {
      //wifi 연결
      Serial.println(F("\r\n---------- AT+CWJAP_DEF ----------\r\n"));
      WIFISerial.print(F("AT+CWJAP_DEF=\""));
      WIFISerial.print(SSID);
      WIFISerial.print(F("\",\""));
      WIFISerial.print(PASSWORD);
      WIFISerial.println(F("\""));
      responseSerial("OK");

      //연결된 ip 출력
      Serial.println(F("\r\n---------- AT+CIFSR ----------\r\n"));
      WIFISerial.println(F("AT+CIFSR"));
      responseSerial("OK");

      //2 반환시 ip할당 성공, 5 반환시 ip할당 실패로 연결 재시도
      if (nowStatus() == 5) {
        continue;
      }
      else {
        return;
      }
    }
  }
}

//Sensor data send
void dataSend(String url) {
  uint8_t startStop = 1;

  while (1) {
    Serial.print(F("\r\n----- AT+CIPMUX -----\r\n"));
    WIFISerial.println(F("AT+CIPMUX=0"));
    responseSerial("OK");

    while (1) {
      Serial.print(F("\r\n----- AT+CIPSTART -----\r\n"));
      WIFISerial.print(F("AT+CIPSTART=\"TCP\",\""));
      WIFISerial.print(IP);
      WIFISerial.print(F("\","));
      WIFISerial.println(PORT);
      responseSerial("OK");

      //TCP가 연결 되었으면 while문 break; 아니면 다시 연결 시도
      if (nowStatus() == 3) {
        break;
      }
      else {
        continue;
      }
    }

    Serial.print(F("\r\n----- AT+CIPSEND -----\r\n"));
    WIFISerial.print(F("AT+CIPSEND="));
    WIFISerial.println(url.length());
    responseSerial("OK\r\n>");

    Serial.print(F("\r\n---------- GET ----------\r\n"));
    WIFISerial.println(url);
    responseSerial("CLOSED");

    startStop = nowStatus();

    //5: ip할당 실패 wifi 연결 시도, 2: ip할당 성공했으나, 중간에 끊긴것임으로 함수 종료
    //3, send 실패 다시 시도, 4 : 정상 send 후 종료
    if (startStop == 5) {
      connectWifi();
      return;
    }
    else if (startStop == 2) {
      return;
    }
    else if (startStop == 3) {
      continue;
    }
    else {
      break;
    }
  }
}

// 현재 상태 출력
uint8_t nowStatus() {
  uint8_t connectionStatus;

  Serial.println(F("\r\n---------- AT+CIPSTATUS ----------\r\n"));
  WIFISerial.println(F("AT+CIPSTATUS"));
  // ':' 가 들어오면 뒤에 숫자 값을 받아 리턴
  if (WIFISerial.find(":")) {
    connectionStatus = WIFISerial.parseInt();
  }

  return connectionStatus;
}

//Serial 출력 및 Command 정상 출력 확인
void responseSerial(char* keyword) {
  uint8_t cutChar = 0;
  uint8_t keywordLength = 0;
  unsigned long startTime;
  int i;

  for (i = 0; keyword[i] != NULL; i++) {
    keywordLength++;
  }

  startTime = millis();
  while ((millis() - startTime) < TIMEOUT) {
    if (WIFISerial.available()) {
      char ch = WIFISerial.read();
      Serial.write(ch);

      if (ch == keyword[cutChar]) {
        cutChar++;
        if (cutChar == keywordLength) {
          return;
        }
      }
      else {
        cutChar = 0;
      }
    }
  }
  return;
}

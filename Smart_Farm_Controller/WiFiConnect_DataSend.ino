/****************************** WIFI Connection ******************************/
void connectWifi() {
  uint8_t connectionStatus;

  while (1) {
    //Wifi 초기화
    Serial.println(F("\r\n---------- AT+RST ----------\r\n"));
    Serial3.println("AT+RST");
    responseSerial("IP");

    //모드 설정(1로 설정)
    Serial.println(F("\r\n---------- AT+CWMODE ----------\r\n"));
    Serial3.println("AT+CWMODE=1");
    responseSerial("OK");

    while (1) {
      //wifi 연결
      Serial.println(F("\r\n---------- AT+CWJAP_DEF ----------\r\n"));
      Serial3.print("AT+CWJAP_DEF=\"");
      Serial3.print(SSID);
      Serial3.print(F("\",\""));
      Serial3.print(PASSWORD);
      Serial3.println(F("\""));
      responseSerial("OK");

      //연결된 IP 출력
      Serial.println(F("\r\n---------- AT+CIFSR ----------\r\n"));
      Serial3.println("AT+CIFSR");
      responseSerial("OK");

      Serial.print(F("\r\n----- AT+CIPMUX -----\r\n"));
      Serial3.println(F("AT+CIPMUX=0"));
      responseSerial("OK");

      //2 반환시 IP할당 성공, 5 반환시 IP할당 실패로 연결 재시도
      if (nowStatus() == 5) {
        continue;
      }
      else {
        return;
      }
    }
  }
}
/****************************** Data Send & Get ******************************/
void GetSetData(String GetSetType, String returnType, String url) {
  uint8_t startStop = 1;

  while (1) {

    while (1) {
      Serial.print(F("\r\n----- AT+CIPSTART -----\r\n"));
      Serial3.print(F("AT+CIPSTART=\"TCP\",\""));
      Serial3.print(IP);
      Serial3.print(F("\","));
      Serial3.println(PORT);
      responseSerial("OK");

      startStop = nowStatus();
      //TCP가 연결 되었으면 while문 break; 아니면 다시 연결 시도
      if (startStop == 3) {
        break;
      }
      else if (startStop == 4) {
        continue;
      }
      else {
        return;
      }
    }

    Serial.print(F("\r\n----- AT+CIPSEND -----\r\n"));
    Serial3.print(F("AT+CIPSEND="));
    Serial3.println(url.length());
    responseSerial("OK\r\n>");

    Serial.print(F("\r\n---------- GET ----------\r\n"));

    //Getter가 들어오면 들어온 값을 넣어줘야하기때문에 구분지어 사용
    if (GetSetType == GetterType) {
      Serial3.println(url);
      getterDataInput();
    }
    else if (GetSetType == SetterType) {
      Serial3.println(url);
      responseSerial("CLOSED");
    }

    startStop = nowStatus();
    //5: IP할당 실패, 2: IP할당 성공했으나, 중간에 끊긴것임으로 함수 종료
    //3, send 실패 다시 시도, 4 : 정상 send 후 종료
    if (startStop == 5) {
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
/****************************** 현재 상태 확인 ******************************/
uint8_t nowStatus() {
  Serial.print(F("\r\n\r\n ========== STATUS ========== \r\n\r\n"));
  uint8_t connectionStatus;

  Serial.println(F("\r\n---------- AT+CIPSTATUS ----------\r\n"));
  //Serial3.println(F("AT+CIPSTATUS"));
  //responseSerial("OK");
  // 'STATUS:' 가 들어오면 뒤에 숫자 값을 받아 리턴

  Serial3.println(F("AT+CIPSTATUS"));
  if (Serial3.find("STATUS:")) {
    connectionStatus = Serial3.parseInt();
  }
  responseSerial("OK");

  return connectionStatus;
}
/****************************** Getter ******************************/
void getterDataInput() {
  int i = 0;
  int16_t number;

  while (Serial3.available()) {
    String bufferData = Serial3.readStringUntil('@');
    Serial.print(F("\r\n bufferData : "));
    Serial.println(bufferData);

    number = bufferData.toInt();

    if (number != 0) {
      getDataValue[i] = number;
      i++;
    }

    if (bufferData == "END") {
      return;
    }
  }
}
/****************************** Response Serial ******************************/
void responseSerial(String keyword) {
  uint8_t cutChar = 0;
  uint8_t keywordLength = keyword.length();
  unsigned long startTime;
  int i;

  startTime = millis();
  while ((millis() - startTime) < TIMEOUT) {
    if (Serial3.available()) {
      char ch = Serial3.read();
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
/****************************** WIFI Connect & Data Get_Set End ******************************/

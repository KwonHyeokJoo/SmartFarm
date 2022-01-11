/****************************** Manual Heater Stop ******************************/
uint8_t manualHeaterControl(uint8_t heaterState) {
  if (heaterState == OFF) {
    //작동 중지 명령
    if (digitalRead(HEATER_PIN) == HIGH) {
      //이미 정지 상태
      Serial.print(F("\r\n === Heater 이미 정지 상태 === \r\n"));
      return OFF;
    }
    else {
      //가동을 정지로 변경
      return OFF;
    }
  }
  else if (heaterState == ON) {
    if (digitalRead(HEATER_PIN) == LOW) {
      // 이미 가동 상태
      Serial.print(F("\r\n === Heater 이미 가동 상태 === \r\n"));
      return ON;
    }
    else {
      // 정지를 가동으로 변경
      return ON;
    }
  }
  else {
    Serial.print(F("\r\n === Manual Heater State Err === \r\n"));
  }
}
/****************************** Auto Heater Control ******************************/
uint8_t autoHeaterControl(uint8_t tempAvg) {
  if (targetTemp > tempAvg) {
    // 설정온도가 평균온도보다 높다면
    if (targetTemp - tempAvg > 3) {
      return ON;
    }
    else {
      return OFF;
    }
  }
}
/****************************** Heater End ******************************/

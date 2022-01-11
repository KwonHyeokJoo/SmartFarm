/****************************** Pump Control ******************************/
uint8_t pumpControl() {
  if (digitalRead(PUMP_PIN) == LOW) {
    if ((endPumpTime - startPumpTime) > waitPumpTime) {
      //20초동안 pump 가동 여부 확인 후 20초 이상이면 pump 종료
      return OFF;
    }
    else {
      endPumpTime = millis();
      return ON;
    }
  }
  else {
    startPumpTime = millis();
    endPumpTime = millis();

    return ON;
  }
}
/****************************** LED Control ******************************/
uint8_t ledControl() {
  if (digitalRead(PUMP_PIN) == LOW) {
    if ((endPumpTime - startPumpTime) > waitPumpTime) {
      return OFF;
    }
    else {
      if ((endLedTime - startLedTime) > waitPumpLedStandard) {
        startLedTime = millis();
        endLedTime = millis();
        return ON;
      }
      else if ((endLedTime - startLedTime) > waitPumpLedTime_3) {
        endLedTime = millis();
        return LED3;
      }
      else if ((endLedTime - startLedTime) > waitPumpLedTime_1) {
        endLedTime = millis();
        return LED1;
      }
      else if ((endLedTime - startLedTime) > waitPumpLedTime_2) {
        endLedTime = millis();
        return LED2;
      }
      else {
        //아무것도 못들어갔다면 end만 초기화
        endLedTime = millis();
      }
    }
  }
  else {
    startLedTime = millis();
    endLedTime = millis();

    return ON;
  }
}
/****************************** Pump & LED Control End ******************************/

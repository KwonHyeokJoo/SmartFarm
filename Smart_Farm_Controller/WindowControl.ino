/****************************** Manual Window Control ******************************/
void manualWindowControl(uint8_t windowControl, uint8_t *windowAngle) {
  if (windowControl == OFF) {
    if (windowAngle == WINDOW_ANGLE_MIN) {
      Serial.print(F("\r\n\r\n === 이미 닫혀 있음 === \r\n\r\n"));
    }
    else {
      //Window 닫기 시작
      if ((windowEndTime - windowStartTime) > (WINDOW_ANGLE_MAX * 10)) {
        windowStartTime = millis();
        *windowAngle = WINDOW_ANGLE_MIN;
      }
      else {
        Serial.print(F("\r\n\r\n === 작동 대기중 === \r\n\r\n"));
        windowEndTime = millis();
      }
    }
  }
  else if (windowControl == ON) {
    //left Window 열기
    if (windowAngle == WINDOW_ANGLE_MAX) {
      Serial.print(F("\r\n\r\n === 이미 열려 있음 === \r\n\r\n"));
    }
    else {
      if ((windowEndTime - windowStartTime) > (WINDOW_ANGLE_MAX * 10)) {
        windowStartTime = millis();
        *windowAngle = WINDOW_ANGLE_MAX;
      }
      else {
        Serial.print(F("\r\n\r\n === 작동 대기중 === \r\n\r\n"));
        windowEndTime = millis();
      }
    }
  }
}
/****************************** Auto Left Window Control ******************************/
uint8_t autoWindowControl(uint8_t tempAvg, uint8_t *windowAngle) {
  if (tempAvg < targetTemp) {
    if ((tempAvg - targetTemp) > 3) {
      //타겟온도보다 평균온도가 3도보다 높으면 열어줌
      *windowAngle = WINDOW_ANGLE_MAX;
      return ON;
    }
    else {
      *windowAngle = WINDOW_ANGLE_MIN;
      return OFF;
    }
  }
  else {
    *windowAngle = WINDOW_ANGLE_MIN;
    return OFF;
  }
}
/****************************** Left Window End ******************************/

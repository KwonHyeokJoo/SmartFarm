/****************************** Manual Fan OnOff ******************************/
uint8_t fanOnOff(uint8_t fanState) {
  if (fanState == ON) {
    return ON;
  }
  else {
    return OFF;
  }
}
/****************************** Manual Fan Stop ******************************/
uint8_t manualFanStop(uint8_t fanState) {
  if (digitalRead(FANCONTROL_IN_1) == LOW) {
    //현재 fan 정지 상태
    Serial.print(F("\r\n\r\n === Fan Already Stop === \r\n\r\n"));
  }
  
  return fanOnOff(fanState);
}
/****************************** Manual Fan Strat ******************************/
uint8_t manualFanStart(uint8_t *fanSpeed, uint8_t *oldFanSpeed) {
  if (digitalRead(FANCONTROL_IN_1) == HIGH) {
    //가동중 속도 조절
    if (*oldFanSpeed != *fanSpeed) {
      if ((endFanSpeedControl - startFanSpeedControl) > waitFanSpeedControl) {
        if (*fanSpeed < FAN_SPEED_MIN) {
          //Fan Speed 최소값 설정(80)
          *fanSpeed = FAN_SPEED_MIN;
          Serial.print(F("\r\n\r\n === Speed MIN === \r\n\r\n"));
        }
        else if (*fanSpeed > FAN_SPEED_MAX) {
          *fanSpeed = FAN_SPEED_MAX;
          Serial.print(F("\r\n\r\n === Speed MAX === \r\n\r\n"));
        }
        else {
          Serial.print(F("\r\n\r\n === Speed ing... === \r\n\r\n"));
        }
      }
      else {
        endFanSpeedControl = millis();
      }
    }
    else {
      Serial.print(F("\r\n\r\n === Fan Working... === \r\n\r\n"));
    }
    return ON;
  }
  else {
    //정지 -> 가동
    *fanSpeed = FAN_SPEED_MAX;
    *oldFanSpeed = *fanSpeed;
    startFanSpeedControl = millis();
    endFanSpeedControl = millis();
    return ON;
  }
}
/****************************** Auto Fan Control ******************************/
uint8_t autoFanControl(int16_t tmp1Data, int16_t tmp2Data, uint8_t *fanSpeed) {
  uint8_t fanState;

  if (tmp1Data > tmp2Data) {
    if ((tmp1Data - tmp2Data) > 5) {
      fanState = ON;
      *fanSpeed = FAN_SPEED_MAX;
    }
    else if ((tmp1Data - tmp2Data) > 3) {
      fanState = ON;
      *fanSpeed = FAN_SPEED_MID;
    }
    else if ((tmp1Data - tmp2Data) > 1) {
      fanState = ON;
      *fanSpeed = FAN_SPEED_MIN;
    }
    else {
      fanState = OFF;
      *fanSpeed = FAN_SPEED_ZERO;
    }
  }
  return fanState;
}
/****************************** Fan Control End ******************************/

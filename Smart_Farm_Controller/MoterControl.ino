/****************************** Moter Control ******************************/
void moterControl(uint8_t moterStatus, uint8_t pin_In_1, uint8_t pin_In_2) {
  //정방향 작동시 입력값 1, 정지 2, 기타 역방향
  if (moterStatus == ON) {
//    Serial.print(F("\r\n === Moter forward === \r\n "));
    digitalWrite(pin_In_1, HIGH);
    digitalWrite(pin_In_2, LOW);
  }
  else if (moterStatus == OFF) {
//    Serial.print(F("\r\n === Moter off === \r\n "));
    digitalWrite(pin_In_1, LOW);
    digitalWrite(pin_In_2, LOW);
  }
  else {
//    Serial.print(F("\r\n === Moter reverse === \r\n"));
    digitalWrite(pin_In_1, LOW);
    digitalWrite(pin_In_2, HIGH);
  }
}
/****************************** Moter Control End ******************************/

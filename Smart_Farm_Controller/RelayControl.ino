/****************************** Relay Control ******************************/
// LOW : on, HIGH : off
void relayControl(uint8_t pinNumber, uint8_t relayOnOff) {
  if (relayOnOff == ON) {
//    Serial.print(F("\r\n=== Relay On ===\r\n"));
    digitalWrite(pinNumber, LOW);
  }
  else {
//    Serial.print(F("\r\n=== Relay Off ===\r\n"));
    digitalWrite(pinNumber, HIGH);
  }
}
/****************************** Relay Control End ******************************/

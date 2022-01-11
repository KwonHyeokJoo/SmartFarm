#include <LiquidCrystal_I2C.h>
#include <Emotion_Farm.h>

#define LCD_X 16                    // LCD X축
#define LCD_Y 2                     // LCD Y축

LiquidCrystal_I2C lcd(0x27, LCD_X, LCD_Y);

void lcd_setup(){
  //LCD 초기화
  lcd.begin();

  // 라이브러리로 추가한 특수 문자 및 이모티콘 추가
  lcd.createChar(0, temp);
  lcd.createChar(1, C);
  lcd.createChar(2, humi);
  lcd.createChar(3, Qmark);
  lcd.createChar(4, water);
  lcd.createChar(5, good);
  lcd.createChar(6, wind);
}

//현재 온도 출력
void printTemp(int8_t temp1, int8_t temp2) {
  //temp 출력을 위한 문자열
  char temp_print[5];

  lcd.setCursor(1, 1);
  lcd.write(0);
  sprintf(temp_print, "%02d", temp1);
  lcd.setCursor(3, 1);
  lcd.print(temp_print);
  lcd.write(1);

  lcd.setCursor(7, 1);
  lcd.write(0);
  sprintf(temp_print, "%02d", temp2);
  lcd.setCursor(9, 1);
  lcd.print(temp_print);
  lcd.write(1);

}

//현재 습도 출력
void printHumi(uint8_t humi1, uint8_t humi2) {
  //humi 출력을 위한 문자열
  char humi_print[5];

  lcd.setCursor(1, 1);
  lcd.write(2);
  sprintf(humi_print, "%02d", humi1);
  lcd.setCursor(3, 1);
  lcd.print(humi_print);
  lcd.print("%");

  lcd.setCursor(7, 1);
  lcd.write(2);
  sprintf(humi_print, "%02d", humi2);
  lcd.setCursor(9, 1);
  lcd.print(humi_print);
  lcd.print("%");

}

//현재 토양수분 출력
void printMoist(uint16_t moist) {
  //moist 수분 출력을 위한 문자열
  char moist_print[5];

  //토양 수분 출력
  lcd.setCursor(1, 0);
  lcd.print("MOIST:");
  lcd.setCursor(8, 0);
  sprintf(moist_print, "%03d", moist);
  lcd.print(moist_print);
  lcd.setCursor(11, 0);
  lcd.print("%");

  //토양습도 값에 따른 LCD에 이모티콘 띄우기
  if (moist >= 0 && moist < 30) {
    lcd.setCursor(13, 0);
    lcd.write(3);
    lcd.setCursor(14, 0);
    lcd.write(4);
  }
  else if (moist >= 30 && moist < 70) {
    lcd.setCursor(13, 0);
    lcd.print(" ");
    lcd.setCursor(14, 0);
    lcd.write(5);
  }
  else if (moist >= 70) {
    lcd.setCursor(13, 0);
    lcd.write(3);
    lcd.setCursor(14, 0);
    lcd.write(6);
  }
}

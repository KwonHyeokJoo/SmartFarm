#include <Adafruit_ESP8266.h>
#include <SoftwareSerial.h>
#include <DHT.h>

//센서 핀 설정
#define soilmoisturePin A0          // 토양수분센서 핀
#define cdsPin A1                   // 조도센서 모듈 핀1
#define DHTPIN_1 4                  // 온습도센서 모듈 핀 1
#define DHTPIN_2 7                  // 온습도센서 모듈 핀 2
#define DHTTYPE DHT11               // 온습도 센서타입 설정
#define WIFI_RX 2                   // WIFI RX
#define WIFI_TX 3                   // WIFI TX

#define TIMEOUT 10000               // TimeOut 시간 설정

//와이파이 통신을 위해 객체생성?
SoftwareSerial WIFISerial(WIFI_RX, WIFI_TX);

// 온습도 센서 객체 생성
DHT dht_1(DHTPIN_1, DHTTYPE);
DHT dht_2(DHTPIN_2, DHTTYPE);

//와이파이 ID 및 Password 설정
const String SSID = "PI_AP";
const String PASSWORD = "12345678";

//서버 IP 설정 및 Port 설정
const String IP = "192.168.1.1";
const String PORT = "80";

//LCD에 온습도 출력을 변환시키기 위해 선언
uint64_t lcdStartTime = 0;
uint64_t lcdEndTime = 0;
uint16_t lcdTempWait = 5000;
uint16_t lcdHumiWait = (lcdTempWait * 2) - 1000;

void setup() {
  Serial.begin(9600);
  WIFISerial.begin(9600);

  Serial.println(F("----- STRAT -----"));

  // LCD 초기화
  lcd_setup();
  
  //wifi connection
  connectWifi();

  pinMode(cdsPin, INPUT);                   //조도 센서
  pinMode(soilmoisturePin, INPUT);          //토양수분센서
}

void loop() {
  //센서값 측정
  uint16_t cdsValue = analogRead(cdsPin);                                   // 조도센서 값 측정: 0(밝음) ~ 1023(어두움)
  uint16_t soilmoistureValue = analogRead(soilmoisturePin);                 // 토양수분 값 측정: 0(습함) ~ 1023(건조)
  uint8_t moist_per = (uint8_t)map(soilmoistureValue, 170, 1023, 100, 0);   // 센서 값을 퍼센트로 변경
  //공기 중 습도 값 측정
  uint8_t humiValue1 = (uint8_t)dht_1.readHumidity();
  uint8_t humiValue2 = (uint8_t)dht_2.readHumidity();
  //공기 중 온도 값 측정
  int8_t tempValue1 = (int8_t)dht_1.readTemperature();
  int8_t tempValue2 = (int8_t)dht_2.readTemperature();
  // 데이터 전송할 dataSendURL
  String dataSendURL;

  //LCD에 현재 온습도 상태 표시
  printMoist(moist_per);

  lcdEndTime = millis();
  //5초 동안 LCD에 온도 표시
  if ((lcdEndTime - lcdStartTime) < lcdTempWait) {
    printTemp(tempValue1, tempValue2);
  }
  //4초 동안 LCD에 습도 표시
  else if ((lcdEndTime - lcdStartTime) < lcdHumiWait) {
    printHumi(humiValue1, humiValue2);
  }
  //습도 표시 이후에 온도 표기를 위해 lcdStartTime에 lcdEndTime 시간 data 저장
  else {
    lcdStartTime = lcdEndTime;
  }

  //Sensing data 전송
  ///////////////////////////////////////////////////////////////////////////////
  dataSendURL = "GET /" + String(moist_per) + "/" + tempValue1 + "/" + tempValue2 + "/" + humiValue1 +
                "/" + humiValue2 + "/" + String(cdsValue) + " HTTP/1.1\r\nHost: " + IP + "\r\n\r\n";
  dataSend(dataSendURL);
  ///////////////////////////////////////////////////////////////////////////////
}

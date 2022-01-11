#include <Servo.h>
#include <Adafruit_ESP8266.h>

// Pin 설정
#define FANCONTROL_IN_1 22          // FAN CONTROL IN 1 설정
#define FANCONTROL_IN_2 24          // FAN CONTROL IN 2 설정
#define FANCONTROL_SPEED 7          // FAN SPEED 설정
#define LEFT_WINDOW_PIN 8           // 좌측 개폐기 설정
#define RIGHT_WINDOW_PIN 9          // 우측 개폐기 설정
#define HEATER_PIN 31               // HEATER 설정
#define LED_PIN 33                  // 식물생장 LED 설정
#define PUMP_PIN 35                 // PUMP 설정
#define PUMP_LED_PIN_1 37           // PUMP LED 1번 설정
#define PUMP_LED_PIN_2 39           // PUMP LED 2번 설정
#define PUMP_LED_PIN_3 41           // PUMP LED 3번 설정

#define TIMEOUT 10000               // TimeOut 시간 설정
#define WINDOW_ANGLE_MAX 90         // 개폐기 최대 각도
#define WINDOW_ANGLE_MIN 0          // 개폐기 최소 각도

#define FAN_SPEED_MAX 255           // FAN의 최대속도
#define FAN_SPEED_MID 128           // FAN의 중간속도
#define FAN_SPEED_MIN 80            // FAN의 최소속도
#define FAN_SPEED_ZERO 0

#define ON 1                        // On 1 return 
#define OFF 2                       // Off 2 return

#define LED1 3                      // PUMP LED 1번 숫자 설정
#define LED2 4                      // PUMP LED 2번 숫자 설정
#define LED3 5                      // PUMP LED 3번 숫자 설정

// 좌우측 개폐기 객체 생성
Servo leftWindow;
Servo rightWindow;

// 와이파이 ID 및 Password 설정
const String SSID = "PI_AP";
const String PASSWORD = "12345678";

// 서버 IP 설정 및 Port 설정
const String IP = "192.168.1.1";
const String PORT = "80";

//Getter, 서버에서 데이터 읽어오기
const uint8_t sensingDataCount = 7;
const uint8_t controlDataCount = 6;
const uint8_t allDataCount = sensingDataCount + controlDataCount;
const String GetterType = "get";
char getterBuffer[60];
String stringGetData[allDataCount];
String getDataType[allDataCount] = {"grd", "tmp1", "tmp2", "hum1", "hum2", "lux", "getTargetTmp",
                                    "getManualControl", "getFanState", "getFanSpeed",
                                    "getLeftWindow", "getRightWindow", "getHeaterState"
                                   };
int16_t getDataValue[allDataCount] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};

//Setter, 서버로 데이터 보내기
const uint8_t setterCount = 4;
const String SetterType = "set";
String setDataType[setterCount] = {"setFan", "setHeat", "setLeftWindow", "setRightWindow"};
int16_t setDataValue[setterCount] = {0, 0, 0, 0};

//fan 시작 시간 측정
uint64_t startFanSpeedControl;
uint64_t endFanSpeedControl;
const uint16_t waitFanSpeedControl = 3000;

// 개폐기 초기 열림 각도 : 최대값으로 설정
int8_t leftWindowAngle = WINDOW_ANGLE_MAX;
int8_t rightWindowAngle = WINDOW_ANGLE_MAX;

//목표 온도, 습도, 조도
uint8_t targetTemp;
uint8_t targetMoist = 40;
uint16_t targetLux = 600;

//개폐기 시간 측정
uint64_t windowStartTime = millis();
uint64_t windowEndTime = millis();
uint64_t rightStartWindowTime;

//히터 시간 측정
uint64_t  startHeaterControl = millis();
uint64_t  endHeaterControl = millis();
const uint16_t waitHeaterControl = 1000;

//pump, led 시간 측정 및 wait 시간
uint64_t startPumpTime = millis();
uint64_t endPumpTime = millis();
uint64_t startLedTime = millis();
uint64_t endLedTime = millis();

const uint16_t waitPumpTime = 20000;          // 펌프 가동시간
const uint16_t waitPumpLedStandard = 9000;    // led 가동 단위시간
const uint16_t waitPumpLedTime_1 = 4000;      // led1 가동시간
const uint16_t waitPumpLedTime_2 = 2000;      // led2 가동시간
const uint16_t waitPumpLedTime_3 = 6000;      // led3 가동시간

/****************************** Setup() ******************************/
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  Serial.println(F("===== START ====="));

  // WIFI Connect
  connectWifi();

  // Servo Moter 핀 설정
  leftWindow.attach(LEFT_WINDOW_PIN);
  rightWindow.attach(RIGHT_WINDOW_PIN);
  
  // Fan Moter 핀 모드 설정
  pinMode(FANCONTROL_IN_1, OUTPUT);
  pinMode(FANCONTROL_IN_2, OUTPUT);
  pinMode(FANCONTROL_SPEED, OUTPUT);

  // Relay 핀 모드 설정
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(PUMP_LED_PIN_1, OUTPUT);
  pinMode(PUMP_LED_PIN_2, OUTPUT);
  pinMode(PUMP_LED_PIN_3, OUTPUT);

  // Relay 초기화
  digitalWrite(HEATER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);
  digitalWrite(PUMP_LED_PIN_1, HIGH);
  digitalWrite(PUMP_LED_PIN_2, HIGH);
  digitalWrite(PUMP_LED_PIN_3, HIGH);
}

/****************************** Loop() ******************************/
void loop() {
  uint8_t manualControl;    // Manial or Auto 변수 선언
  uint8_t oldFanSpeed = 0;  // 이전에 설정된 Fan Speed 변수 선언
  // 각 제어 상태 변수 선언
  uint8_t fanState, fanSpeed, LeftControl, RightControl, heater;
  // 펌프상태 및 LED 변수 선언
  uint8_t pumpState, ledState_1, ledState_2, ledState_3 ;
  // 토양습도, 온도1, 온도2, 습도1, 습도2 값 저장받을 변수 선언
  int16_t grdData, tmp1Data, tmp2Data, hum1Data, hum2Data, luxData;
  String url;               // Data 송수신 할 URL 저장 변수 선언
  int i;                    // 반복문


  /****************************** Server Data Get ******************************/
  url = "GET /getAll HTTP/1.1\r\nHost: " + IP + "\r\n\r\n";
  GetSetData(GetterType, getDataType[i], url);
  /****************************** Get Data 변수에 저장 ******************************/
  for (i = 0; i < allDataCount; i++) {
    if (getDataType[i] == "grd") {
      grdData = getDataValue[i];        // 토양 수분값 저장
    }
    else if (getDataType[i] == "tmp1") {
      tmp1Data = getDataValue[i];       // 온도1 값 저장
    }
    else if (getDataType[i] == "tmp2") {
      tmp2Data = getDataValue[i];       // 온도2 값 저장
    }
    else if (getDataType[i] == "hum1") {
      hum1Data = getDataValue[i];       // 습도1 값 저장
    }
    else if (getDataType[i] == "hum2") {
      hum2Data = getDataValue[i];       // 습도2 값 저장
    }
    else if (getDataType[i] == "lux") {
      luxData = getDataValue[i];        // 조도 값 저장
    }
    else if (getDataType[i] == "getTargetTmp") {
      targetTemp = getDataValue[i];     // 목표 온도 저장
    }
    else if (getDataType[i] == "getManualControl") {
      manualControl = getDataValue[i];  // 수동 & 자동 여부 저장
    }
    else if (getDataType[i] == "getFanState") {
      fanState = getDataValue[i];       // 수동 제어시 Fan 제어 상태 저장
    }
    else if (getDataType[i] == "getFanSpeed") {
      fanSpeed = getDataValue[i];       // 수동 제어시 Fan 속도 상태 저장
    }
    else if (getDataType[i] == "getLeftWindow") {
      LeftControl = getDataValue[i];    // 수동 제어시 Left Window 상태 저장
    }
    else if (getDataType[i] == "getRightWindow") {
      RightControl = getDataValue[i];   // 수동 제어시 Right Window 상태 저장
    }
    else if (getDataType[i] == "getHeaterState") {
      heater = getDataValue[i];         // 수동 제어시 Heater 상태 저장
    }
    else {
      //그 이외의 값이 들어오면 err 띄어주고 다시 Data 받아오기
      Serial.print(F("\r\n\r\n === err === \r\n\r\n"));
    }
  }
  /****************************** Manual Control Start ******************************/
  if (manualControl == OFF) {
    /****************************** Fan Control ******************************/
    if (fanState == OFF) {
      //Fan 정지
      fanState = manualFanStop(fanState);
      moterControl(fanState, FANCONTROL_IN_1, FANCONTROL_IN_2);
    }
    else if (fanState == ON) {
      //Fan 가동
      fanState = manualFanStart(&fanSpeed, &oldFanSpeed);

      moterControl(fanState, FANCONTROL_IN_1, FANCONTROL_IN_2);
      analogWrite(FANCONTROL_SPEED, fanSpeed);
    }
    else {
      Serial.print(F("\r\n === Manual Fan Control Err === \r\n"));
    }
    /****************************** Left Window Control ******************************/
    // Left Window 개폐
    manualWindowControl(LeftControl, &leftWindowAngle);
    leftWindow.write(leftWindowAngle);
    /****************************** Right Window Control ******************************/
    //Right Window 개폐
    manualWindowControl(RightControl, &rightWindowAngle);
    rightWindow.write(rightWindowAngle);
    /****************************** Heater Control ******************************/
    //Heater Control On Off
    heater = manualHeaterControl(heater);
    relayControl(HEATER_PIN, heater);
  }
  /****************************** Manual Control End ******************************/

  /****************************** Auto Control Start ******************************/
  else if (manualControl == ON) {
    uint8_t tempAvg;
    tempAvg = (tmp1Data + tmp2Data) / 2;

    /****************************** Auto Fan Control ******************************/
    //온도 앞 뒤 차이가 5도 이상이면 팬을 가동하여 공기 순환
    fanState = autoFanControl(tmp1Data, tmp2Data, &fanSpeed);

    moterControl(fanState, FANCONTROL_IN_1, FANCONTROL_IN_2);
    analogWrite(FANCONTROL_SPEED, fanSpeed);
    /****************************** Auto heater Control ******************************/
    //목표온도가 현재온도와 3도이상 차이날 경우 히터 On
    heater = autoHeaterControl(tempAvg);
    relayControl(HEATER_PIN, heater);
    /****************************** Auto Window Control ******************************/
    //목표온도가 현재온도와 3도이상 차이날 경우 개폐기 On
    LeftControl = autoWindowControl(tempAvg, &leftWindowAngle);
    leftWindow.write(leftWindowAngle);

    RightControl = autoWindowControl(tempAvg, &rightWindowAngle);
    rightWindow.write(rightWindowAngle);
  }
  /****************************** Auto Control End ******************************/

  /****************************** Pump & LED Control ******************************/
  if (grdData <= targetMoist) {
    //20초동안 pump 가동
    pumpState = pumpControl();

    switch (ledControl()) {
      case ON:
        ledState_1 = ON;
        ledState_2 = ON;
        ledState_3 = ON;
        break;
      case OFF:
        ledState_1 = OFF;
        ledState_2 = OFF;
        ledState_3 = OFF;
        break;
      case LED1:
        ledState_1 = OFF;
        break;
      case LED2:
        ledState_2 = OFF;
        break;
      case LED3:
        ledState_3 = OFF;
        break;
      default:
        Serial.print(F("\r\n === Pump Led Control Err === \r\n"));
    }
    relayControl(PUMP_PIN, pumpState);
    relayControl(PUMP_LED_PIN_1, ledState_1);
    relayControl(PUMP_LED_PIN_2, ledState_2);
    relayControl(PUMP_LED_PIN_3, ledState_3);
  }
  else {
    //수분이 넘었다면 그냥 종료
    pumpState = OFF;
    ledState_1 = OFF;
    ledState_2 = OFF;
    ledState_3 = OFF;

    relayControl(PUMP_PIN, pumpState);
    relayControl(PUMP_LED_PIN_1, ledState_1);
    relayControl(PUMP_LED_PIN_2, ledState_2);
    relayControl(PUMP_LED_PIN_3, ledState_3);
  }
  /****************************** 식물 생장 LED Control ******************************/
  if (luxData < targetLux) {
    relayControl(LED_PIN, OFF);
  }
  else if (luxData >= targetLux) {
    relayControl(LED_PIN, ON);
  }
  /****************************** Server Data Send ******************************/
  //     /controllerSetter/우측/좌측/열풍기/팬
  url = "GET /controllerSetter/" + String(LeftControl) + "/" + String(RightControl) + "/" + String(heater) + "/" + String(fanState) + " HTTP/1.1\r\nHost: " + IP + "\r\n\r\n";
  GetSetData(SetterType, setDataType[i], url);
  /****************************** Loop End ******************************/
}

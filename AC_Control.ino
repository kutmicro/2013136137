#include <LiquidCrystal.h>
// Select the pin used on LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0 // 증가 →
#define btnUP     1 // MAX설정 ↑
#define btnDOWN   2 // MIN설정 ↓
#define btnLEFT   3 // 감소 ←
#define btnSELECT 4 // 세팅완료
#define btnNONE   5 // 입력 X

float MAX_Temp = 28.0; // AC가 켜지는 온도
float MIN_Temp = 23.0; // AC가 꺼지는 온도
float Select = 0; // MAX or MIN 선택(MAX → 0 , MIN → 1)
int SettingComplete = 0; // 초기 세팅제어

int IRledPin = 40; // IR LED 디지털핀 40
int TempPin = A10; // 온도센서 아날로그핀 10
int LightPin = A15; // 조도센서 아날로그핀 15
int On = 0; // 초기 AC의 전원은 Off로 설정

int TurnOn[]   = {8996,-4456,604,-500,604,-492,612,-1596,612,-1592,612,-1596,612,-492,612,-1592,612,-1596,612,-496,612,-1588,620,-1588,620,-1588,620,-484,620,-488,612,-1596,620,-1588,612,-1592,620,-1584,612,-492,616,-496,608,-496,624,-488,616,-496,616,-496,620,-484,608,-492,616,-1592,608,-1592,620,-1592,620,-1588,620,-1588,612,-1596,616,0};
int TurnOff[]  = {8992,-4448,616,-496,608,-504,612,-1596,616,-1588,624,-1588,616,-496,616,-1596,616,-1596,608,-504,616,-1588,624,-1588,612,-1592,616,-492,616,-496,616,-1588,616,-1596,616,-1596,612,-1596,608,-492,616,-492,616,-496,616,-492,612,-492,612,-484,620,-488,624,-488,616,-1588,616,-1596,616,-1588,624,-1588,616,-1596,612,-1596,604,0};
int TempUp[]   = {8992,-4452,620,-488,612,-492,612,-1592,612,-1592,608,-1596,616,-496,620,-1588,612,-1588,616,-496,616,-1596,616,-1596,616,-1588,616,-496,616,-496,608,-1596,612,-1596,608,-1584,612,-492,612,-1592,620,-488,616,-1588,620,-488,624,-488,616,-492,612,-492,620,-1588,612,-492,608,-1592,620,-488,616,-1596,604,-1596,604,-1596,612,0};
int TempDown[] = {9000,-4448,612,-492,624,-488,612,-1596,612,-1588,624,-1588,620,-488,620,-1588,616,-1596,620,-484,620,-1588,620,-1588,620,-1584,612,-492,616,-492,616,-1596,616,-1596,612,-1596,612,-484,612,-492,620,-1588,624,-1588,616,-496,616,-496,608,-504,612,-496,604,-1596,616,-1596,624,-484,612,-492,620,-1588,612,-1596,612,-1596,612,0};

int Read_LCD_Buttons(); // 버튼입력
void IRsend(int IRsignal[]); // 적외선 송신

void setup() {
  Serial.begin(9600); // 시리얼 통신속도 설정
  pinMode(IRledPin, OUTPUT);
  IRsend(TurnOff);

  lcd.begin(16, 2); // LCD 크기 설정
  lcd.setCursor(14,0);
  lcd.print("<<");
}

void loop() {

  lcd.setCursor(0,0); // 커서 설정
  lcd.print("MAXTemp : ");
  lcd.print(MAX_Temp, 1);
  lcd.setCursor(0,1); // 커서 설정
  lcd.print("MINTemp : ");
  lcd.print(MIN_Temp, 1);

  switch ( Read_LCD_Buttons() ) { // 버튼 입력
  case btnRIGHT: { // 증가 →
    if (Select == 0)
      MAX_Temp += 0.1;
    else
      MIN_Temp += 0.1;
    delay(100);
    break;
    }
  case btnLEFT: { // 감소 ←
    if (Select == 0)
      MAX_Temp -= 0.1;
    else
      MIN_Temp -= 0.1;
    delay(100);
    break;
    }
  case btnUP: { // MAX설정 ↑
    Select = 0;
    lcd.setCursor(14,0);
    lcd.print("<<");
    lcd.setCursor(14,1);
    lcd.print("  ");
    break;
    }
  case btnDOWN: { // MIN설정 ↓
    Select = 1;
    lcd.setCursor(14,1);
    lcd.print("<<");
    lcd.setCursor(14,0);
    lcd.print("  ");
    break;
    }
  case btnSELECT: { // 세팅완료버튼
    lcd.clear(); // lcd의 모든내용 삭제
    SettingComplete = 1; // 세팅완료
    lcd.setCursor(0,0);
    lcd.print("  Setting");
    lcd.setCursor(0,1);
    lcd.print("    Complete!!");
    delay(5000);
    lcd.clear(); // lcd의 모든내용 삭제
    break;
    }
  case btnNONE: { // 입력 X
    break;
    }
  }

  while (SettingComplete == 1) { // 세팅이 완료되면 AC 컨트롤 시작
    // 밝을수록 0에 가깝고 어두울수록 1023에 가까움
    int lightValue = analogRead(LightPin); // 조도 센서값 읽어옴
    float sensorValue = analogRead(TempPin); // 온도 센서값 읽어옴
    float temperature = (sensorValue/1023)*5*100; // 온도 센서값(0 ~ 1023) 섭씨온도로 변환
    // LM35 계산 공식에 의함, 5는 전원전압(V)

    Serial.print("The room temperature degree is : ");
    Serial.println(temperature,1);
    lcd.setCursor(0,0); // 커서 설정
    lcd.print("  Temp : ");
    lcd.print(temperature,1);

    Serial.print("Current ambient light level is : ");
    Serial.println(lightValue);
    lcd.setCursor(0,1); // 커서 설정
    lcd.print(" Light : ");
    lcd.print(lightValue);

    delay(3000); // 3초 대기

    if (lightValue < 360){ // 낮 ☞ 이 시간에만 에어컨 가동

      while (On == 0){ // AC의 전원이 꺼져 있을 때

        sensorValue = analogRead(TempPin); // 온도 센서값 읽어옴
        temperature=(sensorValue/1023)*5*100; // 온도 센서값(0 ~ 1023) 섭씨온도로 변환
        lightValue = analogRead(LightPin);  // 조도 센서값 읽어옴

        lcd.setCursor(0,0); // 커서 설정
        lcd.print("  Temp : ");
        lcd.print(temperature,1);
        lcd.setCursor(0,1); // 커서 설정
        lcd.print(" Light : ");
        lcd.print(lightValue);

        if (lightValue > 360){ // 낮 → 초저녁
          break;
        }
        else if(temperature > MAX_Temp){
          Serial.println("Too hot! Turning on AC");
          IRsend(TurnOn); // 전원 On
          On = 1;
          Serial.println("AC is on now. Waiting 5 minutes for the room to cool down...");
          lcd.setCursor(0,1); // 커서 설정
          lcd.print("Hot!! Turn On AC");
          delay(300000);  // 5분 대기
        }
        else if (temperature < MIN_Temp){
          Serial.print("Too cold! The current temperature degree is:");
          Serial.println(temperature,1);
          IRsend(TurnOff); // 전원 Off
          On = 0;
          lcd.setCursor(0,1); // 커서 설정
          lcd.print("Turn Off AC");
          delay(900000); // 15분 후에 온도, 조도 다시 측정
        }
        else {
          Serial.print("Not too hot yet. Waiting... The room temperature degree is:");
          Serial.println(temperature,1);
          delay(10000); // 10초 대기
        }
      }

      while (On == 1){ // AC의 전원이 켜져 있을 때

        sensorValue = analogRead(TempPin); // 온도 센서값 읽어옴
        temperature=(sensorValue/1023)*5*100; // 온도 센서값(0 ~ 1023) 섭씨온도로 변환
        lightValue = analogRead(LightPin);  // 조도 센서값 읽어옴

        lcd.setCursor(0,0); // 커서 설정
        lcd.print("  Temp : ");
        lcd.print(temperature,1);
        lcd.setCursor(0,1); // 커서 설정
        lcd.print(" Light : ");
        lcd.print(lightValue);

        if (lightValue > 360){ // 낮 → 초저녁
          break;
        }
        else if (temperature < MIN_Temp+1 && temperature > MIN_Temp){ // 23.X ℃
          Serial.println("Too cold! Increasing Temperature");
          IRsend(TempUp);
          lcd.setCursor(0,1); // 커서 설정
          lcd.print("Cold!! Temp Up");
          delay(300000); // 5분 대기
        }
        else if (temperature > MAX_Temp){
          Serial.println("Hoooot! Decreasing Temperature");
          IRsend(TempDown);
          lcd.setCursor(0,1); // 커서 설정
          lcd.print("Hot!! Temp Down");
          delay(300000); // 5분 대기
        }
        else if (temperature < MIN_Temp){
          Serial.println("Too Cold in here. Turning off AC");
          IRsend(TurnOff); // 전원 Off
          On = 0;
          lcd.setCursor(0,1); // 커서 설정
          lcd.print("TooCold Turn Off");
          delay(600000); // 10분 대기
        }
        else {
          Serial.print("The room temperature degree is:");
          Serial.println(temperature,1);
          delay(10000); // 10초 대기
        } 
      }
    }

    else if (lightValue > 360 && lightValue < 700){ // 초저녁이나 이른 아침
      Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
      Serial.println(lightValue);
      lcd.setCursor(0,0); // 커서 설정
      lcd.print("AC is Off");
      lcd.setCursor(0,1); // 커서 설정
      lcd.print("Early Evening");
      
      IRsend(TurnOff); // 전원 Off
      On = 0;
      delay(900000); // 15분 후에 조도 다시 측정
      lightValue = analogRead(LightPin);
    }

    else { // 한밤중
      Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
      Serial.println(lightValue);
      lcd.setCursor(0,0); // 커서 설정
      lcd.print("AC is Off");
      lcd.setCursor(0,1); // 커서 설정
      lcd.print("Evening");
      
      IRsend(TurnOff); // 전원 Off
      On = 0;
      delay(3600000); // 1시간 후에 조도 다시 측정
      lightValue = analogRead(LightPin);
    }
  }
}

int Read_LCD_Buttons() { // 버튼 입력
  int adc_key_in = analogRead(A0); // read analog A0 value 
  // when read the 5 key values in the vicinity of the following：0,144,329,504,741
  // By setting different threshold, you can read the one button
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  return btnNONE; 
}

// 데이터를 38kHz로 변조하여 적외선 신호로 송신
void IRsend(int IRsignal[]) {
  int i=0;
  do {
    if (IRsignal[i] > 0) {
      int duration = IRsignal[i];
      while (duration > 0) {
        digitalWrite(IRledPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(IRledPin, LOW);
        delayMicroseconds(10);
        duration -= 26;         // 38 kHz = 26.3 uSec
      }
    }
    else {
      delayMicroseconds(abs(IRsignal[i]));
    }
    i+=1;
  }
  while (IRsignal[i] != 0);
}

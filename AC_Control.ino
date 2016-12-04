#include <LiquidCrystal.h>
// Select the pin used on LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int lcd_key     = 0;
int adc_key_in  = 0;
int SettingComplete = 0; // 초기 세팅제어

float MAX_Temp = 28.0; // AC가 켜지는 온도
float MIN_Temp = 23.0; // AC가 꺼지는 온도
float Select = 0; // 설정시 MAX or MIN 선택

int IRledPin = 40; // IR LED 디지털핀 40
int TempPin = A10; // 온도센서 아날로그핀 10
int LightPin = A15; // 조도센서 아날로그핀 15
int On = 0; // 초기 AC의 전원은 Off로 설정

int TurnOn[] = {4588,-4352,648,-1580,600,-484,600,-512,560,-520,588,-516,588,-520,564,-520,588,-516,564,-548,556,-1660,564,-520,576,-520,560,-1656,592,-1632,580,-1628,592,-1632,580,-1660,552,-1664,552,-532,576,-520,560,-18796,4520,-4412,572,-1624,592,-520,564,-520,588,-516,556,-548,564,-520,592,-520,564,-516,584,-520,588,-1632,588,-512,568,-516,596,-1652,564,-1648,564,-1648,560,-1656,564,-1652,596,-1620,592,-520,564,-520,584,0};
int TurnOff[] = {4588,-4352,648,-1580,600,-484,600,-512,560,-520,588,-516,588,-520,564,-520,588,-516,564,-548,556,-1660,564,-520,576,-520,560,-1656,592,-1632,580,-1628,592,-1632,580,-1660,552,-1664,552,-532,576,-520,560,-18796,4520,-4412,572,-1624,592,-520,564,-520,588,-516,556,-548,564,-520,592,-520,564,-516,584,-520,588,-1632,588,-512,568,-516,596,-1652,564,-1648,564,-1648,560,-1656,564,-1652,596,-1620,592,-520,564,-520,584,0};
int TempUp[]  = {4696,-4212,792,-1424,752,-344,728,-384,676,-412,684,-424,680,-436,636,-456,632,-476,588,-532,560,-1648,564,-520,580,-528,556,-1660,580,-1632,588,-1624,600,-1624,584,-1648,564,-1648,564,-520,588,-524,552,-18808,4520,-4412,572,-1620,584,-512,568,-516,600,-512,560,-548,564,-520,588,-512,564,-520,592,-508,592,-1632,580,-520,560,-520,588,-1656,572,-1644,568,-1652,564,-1648,564,-1648,600,-1612,600,-512,568,-512,588,0};
int TempDown[] = {4592,-4336,664,-1556,616,-464,624,-492,568,-520,592,-520,592,-520,552,-520,592,-520,560,-552,560,-1656,572,-512,588,-512,568,-1652,592,-1628,584,-1628,588,-1616,596,-1652,552,-1672,564,-508,592,-520,564,-18784,4540,-4404,580,-1612,592,-516,564,-520,584,-528,564,-540,560,-520,592,-520,564,-520,588,-524,580,-1628,592,-520,564,-520,592,-1648,572,-1640,572,-1644,568,-1652,564,-1648,592,-1620,592,-516,560,-524,588,0};

int Read_LCD_Buttons();
void IRsend(int IRsignal[]);

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

  lcd_key = Read_LCD_Buttons(); // 버튼 입력
  switch (lcd_key) {
  case btnRIGHT: {
    if (Select == 0)
      MAX_Temp += 0.1;
    else
      MIN_Temp += 0.1;
    delay(100);
    break;
    }
  case btnLEFT: {
    if (Select == 0)
      MAX_Temp -= 0.1;
    else
      MIN_Temp -= 0.1;
    delay(100);
    break;
    }
  case btnUP: {
    Select = 0;
    lcd.setCursor(14,0);
    lcd.print("<<");
    lcd.setCursor(14,1);
    lcd.print("  ");
    break;
    }
  case btnDOWN: {
    Select = 1;
    lcd.setCursor(14,1);
    lcd.print("<<");
    lcd.setCursor(14,0);
    lcd.print("  ");
    break;
    }
  case btnSELECT: {
    lcd.clear(); // lcd의 모든내용 삭제
    SettingComplete = 1; // 세팅 완료
    lcd.setCursor(0,0);
    lcd.print("  Setting");
    lcd.setCursor(0,1);
    lcd.print("    Complete!!");
    delay(5000);
    lcd.clear(); // lcd의 모든내용 삭제
    break;
    }
  case btnNONE: {
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

        if (lightValue > 360){ // 낮 → 초저녁
          break;
        }
        else if(temperature > MAX_Temp){
          Serial.println("Too hot! Turning on AC");
          IRsend(TurnOn); // 전원 On
          On = 1;
          Serial.println("AC is on now. Waiting 5 minutes for the room to cool down...");
          delay(300000);  // 5분 대기
        }
        else if (temperature < MIN_Temp){
          Serial.print("Too cold! The current temperature degree is:");
          Serial.println(temperature,1);
          IRsend(TurnOff); // 전원 Off
          On = 0;
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

        if (lightValue > 360){ // 낮 → 초저녁
          break;
        }
        else if (temperature < MIN_Temp+1 && temperature > MIN_Temp){ // 23.X ℃
          Serial.println("Too cold! Increasing Temperature");
          IRsend(TempUp);
          delay(300000); // 5분 대기
        }
        else if (temperature > MAX_Temp){
          Serial.println("Hoooot! Decreasing Temperature");
          IRsend(TempDown);
          delay(300000); // 5분 대기
        }
        else if (temperature < MIN_Temp){
          Serial.println("Too Cold in here. Turning off AC");
          IRsend(TurnOff); // 전원 Off
          On = 0;
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
      IRsend(TurnOff); // 전원 Off
      On = 0;
      delay(900000); // 15분 후에 조도 다시 측정
      lightValue = analogRead(LightPin);
    }

    else { // 한밤중
      Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
      Serial.println(lightValue);
      IRsend(TurnOff); // 전원 Off
      On = 0;
      delay(3600000); // 1시간 후에 조도 다시 측정
      lightValue = analogRead(LightPin);
    }
  }
}

int Read_LCD_Buttons() { // 버튼 입력
  adc_key_in = analogRead(A0); // read analog A0 value 
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

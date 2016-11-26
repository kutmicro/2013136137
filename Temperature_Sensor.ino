/*
LM35D temperature sensor read test
*/

const int temperaturePin = 0;
 
void setup() {
  Serial.begin(9600);  // 시리얼 통신속도 설정
}

void loop() {
  int reading = analogRead(temperaturePin);  // 센서값 읽어옴
 
  Serial.print( (5.0 * reading * 100.0) / 1024.0 );  // 센서값(0 ~ 1023) 섭씨온도로 변환(LM35 계산 공식에 의함)
  Serial.println(" C");
  
  delay(1000);  // 측정 간격 설정 (1000 = 1초)
}

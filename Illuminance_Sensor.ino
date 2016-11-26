#define PIN A0

void setup() {
  
  Serial.begin(9600);   //시리얼 통신속도 설정
  
}
void loop() {
  
  int val = analogRead(PIN); // 센서값 읽어옴

  Serial.println(val);  // 조도센서 값(0 ~ 1023) 출력

  delay(300);            // 측정 간격 설정 (1000 = 1초)
  
}

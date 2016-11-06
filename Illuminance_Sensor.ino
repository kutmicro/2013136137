#define POT_PIN A0 
#define LED_PIN 13 

void setup()
{
  Serial.begin(9600);  //시리얼 통신속도 설정
  pinMode(LED_PIN, OUTPUT); 
}
void loop()
{
  int val = 0;
  val = analogRead(POT_PIN); 

  Serial.println(val); // 조도센서 값 출력

  //조도센서 값이 100미만이면 LED를 켠다
  if(val < 100) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(500);
}

/*
  Measure current temperature and ambient light level, then determine whether to turn on or off A/C.
  
  Temperature Measurement
  Reads an analog input on A0(output from LM35DZ), change it to the real room temperature and print it to serial monitor.
  
  Ambient Light Measurement
  Reads an analog input on A3
 */
 
int IRledPin =  13;    // IR LED connected to digital pin 13
float powervoltage=5;//define the power supply voltage.
int On = 0; //Initial state of A/C should be OFF

void pulseIR(long microsecs);
void TurnOn();
void TurnOff();
void TempUp();
void TempDown();

void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(IRledPin, OUTPUT);   
  Serial.begin(9600);
}

void loop() {
  //Testing if On & Off codes work
  //delay(5000);
  //Serial.println("Turn On");  
  //SendChannelUpCode();
  //delay(5000);
  //Serial.println("Turn Off");  
  //TurnOffCode();
  //delay(5000);
  
  // read the input on analog pin 3 for ambient light
  int lightValue = analogRead(A3);
  
  // read the input on analog pin 1 for temperature
  float temperature;
  float sensorValue = analogRead(A1);
  temperature=(sensorValue/1023)*powervoltage*100;
  
  // print out initial conditions
  Serial.print("The room temperature degree is:");
  Serial.println(temperature,1);
  Serial.print("Current ambient light level is:");
  Serial.println(lightValue);
  delay(5000);  

/*
  Turn on AC only during the day or when lights are on.
  AC turns on at a room temperature higher than 28.9, turns off at lower than 23.2.
*/

  // Only turn on AC during the day.
  if (lightValue < 360){
    while (On == 0){
      sensorValue = analogRead(A1);
      temperature=(sensorValue/1023)*powervoltage*100;
      lightValue = analogRead(A3);
      if (lightValue > 360){
        Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
        Serial.println(lightValue);
        TurnOff();
        delay(900000); //Wait for 15 minutes to check ambient light
      }
      else if(temperature > 28.9){
        Serial.println("Too hot! Turning on AC");
        TurnOn(); //Turn on AC
        Serial.println("AC is on now. Waiting 5 minutes for the room to cool down...");
        delay(300000);  // wait 5 minutes
        On = 1;
      }
      else if (temperature < 23.7){
        Serial.print("Too cold! The current temperature degree is:");
        Serial.println(temperature,1);
        TurnOff();
        delay(900000); //Wait for 15 minutes to check ambient light
      }  
      else {
        Serial.print("Not too hot yet. Waiting... The room temperature degree is:");
        Serial.println(temperature,1);
        delay(10000); // Polling every 10 seconds
      }
    }
   
    while (On == 1){
     sensorValue = analogRead(A1);
     temperature=(sensorValue/1023)*powervoltage*100;
     lightValue = analogRead(A3);
     if (lightValue > 360){
       Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
       Serial.println(lightValue);
       TurnOff();
       On = 0;
       delay(900000); //Wait for 15 minutes to check ambient light
     }
     else if (temperature < 24 && temperature > 23){
       Serial.println("Too cold! Increasing Temperature");
       TempUp();
       delay(300000);
     }
     else if (temperature > 28.0){
       Serial.println("Hoooot! Decreasing Temperature");
       TempDown();
       delay(300000);
     }
     else if (temperature < 23.2){
       Serial.println("Too Cold in here. Turning off AC");
       TurnOff();
       On = 0;
       delay(600000);
     }
     else {
       Serial.print("The room temperature degree is:");
       Serial.println(temperature,1);
       delay(10000); // Polling every 10 seconds
     } 
   }
  }
  
  else if (lightValue > 360 && lightValue < 700){
    Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
    Serial.println(lightValue);
    TurnOff();
    On = 0;
    delay(900000); //Wait for 15 minutes to check ambient light
    lightValue = analogRead(A3);
  }
  
  else {
    Serial.print("It is too dark to turn on AC right now. Current ambient light level is:");
    Serial.println(lightValue);
    TurnOff();
    On = 0;
    delay(3600000); //Wait for 1 hr to check ambient light
    lightValue = analogRead(A3);
  }
}

// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}
 
void TurnOn() {
  delayMicroseconds(31748); //Time off (Left Column on serial monitor)
  pulseIR(2940);           //Time on  (Right Column on serial monitor)
  delayMicroseconds(8880);
  pulseIR(500);
  delayMicroseconds(1520);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(520);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(520);
  delayMicroseconds(1500);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(500);
  delayMicroseconds(1520);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(520);
  delayMicroseconds(1540);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1600);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(520);
  delayMicroseconds(540);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(520);
  delayMicroseconds(1480);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(500);
  delayMicroseconds(1520);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(520);
  delayMicroseconds(1500);
  pulseIR(520);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(440);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(520);
  delayMicroseconds(540);
  pulseIR(440);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(600);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(460);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(1560);
  pulseIR(440);
  delayMicroseconds(1560);
  pulseIR(640);
}

void TempUp() {
  // This is the code for the CHANNEL + for the downstairs TV COMCAST
  delayMicroseconds(13712); //Time off (Left Column on serial monitor)
  pulseIR(2920);           //Time on  (Right Column on serial monitor)
  delayMicroseconds(8880);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(1520);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(540);
  delayMicroseconds(1540);
  pulseIR(540);
  delayMicroseconds(500);
  pulseIR(540);
  delayMicroseconds(1480);
  pulseIR(500);
  delayMicroseconds(1540);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(520);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(1540);
  pulseIR(460);
  delayMicroseconds(1560);
  pulseIR(500);
  delayMicroseconds(1520);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(560);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(1560);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(520);
  delayMicroseconds(540);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(460);
  delayMicroseconds(1540);
  pulseIR(460);
  delayMicroseconds(600);
  pulseIR(480);
  delayMicroseconds(1600);
  pulseIR(520);
  delayMicroseconds(1480);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(480);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(1560);
  pulseIR(500);
  delayMicroseconds(1520);
  pulseIR(440);
}

void TempDown() {
  // This is the code for the CHANNEL + for the downstairs TV COMCAST
  delayMicroseconds(56600); //Time off (Left Column on serial monitor)
  pulseIR(2940);           //Time on  (Right Column on serial monitor)
  delayMicroseconds(8900);
  pulseIR(480);
  delayMicroseconds(1560);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(520);
  delayMicroseconds(580);
  pulseIR(480);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(480);
  delayMicroseconds(1520);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(520);
  delayMicroseconds(1480);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(480);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(480);
  delayMicroseconds(1580);
  pulseIR(500);
  delayMicroseconds(1540);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(440);
  delayMicroseconds(580);
  pulseIR(460);
  delayMicroseconds(1540);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(440);
  delayMicroseconds(580);
  pulseIR(500);
  delayMicroseconds(1500);
  pulseIR(480);
  delayMicroseconds(1560);
  pulseIR(500);
  delayMicroseconds(520);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(560);
  delayMicroseconds(1500);
  pulseIR(520);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(440);
  delayMicroseconds(1560);
  pulseIR(460);
  delayMicroseconds(1580);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(520);
  delayMicroseconds(1500);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(500);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(560);
  pulseIR(440);
  delayMicroseconds(540);
  pulseIR(480);
  delayMicroseconds(580);
  pulseIR(460);
  delayMicroseconds(560);
  pulseIR(500);
  delayMicroseconds(560);
  pulseIR(460);
  delayMicroseconds(580);
  pulseIR(440);
  delayMicroseconds(1560);
  pulseIR(480);
  delayMicroseconds(1540);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(440);
  delayMicroseconds(1540);
  pulseIR(580);
}

void TurnOff() {
  delayMicroseconds(23516); //Time off (Left Column on serial monitor)
  pulseIR(3100);           //Time on  (Right Column on serial monitor)
  delayMicroseconds(8620);
  pulseIR(520);
  delayMicroseconds(1540);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(1540);
  pulseIR(500);
  delayMicroseconds(1540);
  pulseIR(500);
}

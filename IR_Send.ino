int testSignal[] = {9000,-4500,500,-500,500,-500,500,-1500,500,-500,500,-500,500,-500,500,-500,500,-500,500,-1500,500,-1500,500,-500,500,-1500,500,-1500,500,-1500,500,-1500,500,-1500,500,-500,500,-500,500,-1500,500,-500,500,-500,500,-500,500,-1500,500,-500,500,-1500,500,-1500,500,-500,500,-1500,500,-1500,500,-1500,500,-500,500,-1500,500,0};
int onSignal[]  = {3100,-9800,470,-1550,470,-550,470,-550,470,-550,470,-1550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-1550,470,-1550,470,-550,470,-550,470,-550,470,-1550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,0};
int offSignal[] = {3100,-9800,470,-1550,470,-550,470,-550,470,-550,470,-1550,470,-550,470,-550,470,-550,470,-1550,470,-1550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-550,470,-1550,470,-550,470,-1550,470,-550,470,-550,470,-550,470,-1550,470,0};
 
void setup() {
}
 
void loop() {
  IRsend(5,testSignal);      // GPIO pin 5 (D1 of NodeMCU, WeMos D1 Mini)
  delay(5000);
  IRsend(5,onSignal);
  delay(5000);
  IRsend(5,offSignal);
  delay(5000);
}
 
void IRsend(int IRpin, int IRsignal[]) {
  int i=0;
  pinMode(IRpin, OUTPUT);
  do {
    if (IRsignal[i]>0) {
      int duration = IRsignal[i];
      while (duration > 0) {
        digitalWrite(IRpin, HIGH);
        delayMicroseconds(10);
        digitalWrite(IRpin, LOW);
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

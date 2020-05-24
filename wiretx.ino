
void transmit() {
 if(sensorid != 300){
  for (int i=0;i<2;i++){
    if(i==1){  
      Wire.beginTransmission(9); // transmit to device #9
      Wire.write(sensorid);              // sends x 
      Wire.endTransmission();    // stop transmitting
    }
 }
}}
void blinkred()
{
  now = millis();
  if(now - prev >= period)
  {
    boolean state = digitalRead(red);
    digitalWrite(red, !state);
    prev = now;
  }
}

void blinkgreen()
{
  nowg = millis();
  if(nowg - prevg >= periodg)
  {
    boolean stateg = digitalRead(green);
    digitalWrite(green, !stateg);
    prevg = nowg;
  }
}

void buzz(){
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
  delay(10);
}
void buzz2(){
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
}
void buzz3(){
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(50);
}

void beepAfter1minute(){
  minNow = millis();
  digitalWrite(buzzer, LOW);

  if (minNow - minPrev >= minT){
    minState = !minState;
    minPrev = minNow;
  }
 
  if (minState == !minState){
    for (size_t i = 0; i < 4; i++)
    {
      beepNow = millis();
      if (beepNow - beepPrev >= beepT)
      {
        digitalWrite(buzzer, !(digitalRead(buzzer)));
        beepPrev = beepNow;
      }
    }
  }
}

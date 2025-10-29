const unsigned char interruptPin = 7;
const unsigned char ledPin = 13;
volatile bool stopped = false;
volatile unsigned long last_stop = 0;
unsigned long time = millis();

void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), disable_isr, FALLING);

}

void loop() {
  if(!stopped){
    if(millis()-time>500){
      digitalWrite(ledPin,!digitalRead(ledPin));
      time = millis();
    }
  }else{
    digitalWrite(ledPin,LOW);
  }
}

void disable_isr() {
  if(micros() - last_stop>500000){
    stopped = !stopped;
    last_stop = micros();
  }
}

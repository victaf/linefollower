void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(9600);
  Serial.println("Ready to connect\nDefualt password is 1234 or 0000"); 
}

void loop() {
  String woord;
  if (Serial1.available()) {
    woord = Serial1.readStringUntil('\n');
    Serial.println(woord);
  }
  if(woord == "aan"){
    digitalWrite(LED_BUILTIN,HIGH);
  }else if(woord == "uit"){
    digitalWrite(LED_BUILTIN,LOW);
  }
  
}
#define RELAY_PIN 6  

void setup() {
  Serial.begin(9600);           
  pinMode(RELAY_PIN, OUTPUT);   
  digitalWrite(RELAY_PIN, LOW); 
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  

    if (command == "ON") {
      digitalWrite(RELAY_PIN, HIGH);  
    }
    else if (command == "OFF") {
      digitalWrite(RELAY_PIN, LOW);   
    }
  }
}

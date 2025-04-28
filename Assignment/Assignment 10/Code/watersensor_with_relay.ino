#define RELAY_PIN 7
#define WATER_SENSOR_PIN A0

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  static unsigned long lastSend = 0;
  unsigned long now = millis();

  // send data for 1 second
  if (now - lastSend > 1000) {
    int sensorValue = analogRead(WATER_SENSOR_PIN);
    String status = (sensorValue < 500) ? "Wet" : "Dry";  // can change

    Serial.println(status);  // send to PC
    lastSend = now;
  }

  // turn on/off relay from pc
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "ON") {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else if (command == "OFF") {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}


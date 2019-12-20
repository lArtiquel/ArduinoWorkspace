
byte incomingByte;
const byte RED = 13;
const byte GREEN = 7;

void setup() {
  Serial.begin(9600);
  pinMode (RED, OUTPUT);
  pinMode (GREEN, OUTPUT);
}

void loop() {
    if (Serial.available() > 0) {
    incomingByte = Serial.read();

    if (incomingByte == '1') {
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, HIGH);
    }
    else if (incomingByte == '0') {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
    }
  }
  delay(50);
}

const int ledPin =  13;     // the number of the LED pin
const int buttonPin = 22;   // the number of the button pin

void setup() {
  // initialize UART0
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  // initialize button pin
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // read from port 0:
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0xA1) {
      digitalWrite(ledPin, HIGH);
    } else if (inByte == 0xB1) {
      digitalWrite(ledPin, LOW);
    }
  }

  // write to port 0:
  if(digitalRead(buttonPin)==LOW){
    Serial.write(0xB1);
    delay(200);
  }
}

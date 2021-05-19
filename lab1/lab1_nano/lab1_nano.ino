 const int buttonPin = 12;

const byte animationLed[4] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000
};

void setup() {
  DDRD = 0xFF;
  PORTD=0;
  pinMode(buttonPin,INPUT_PULLUP);
}

void loop() {
 
  if(digitalRead(buttonPin) == LOW){
    for(int i = 0; i<4;i++){
     PORTD = animationLed[i]; 
     delay(900);
    }
    PORTD = 0;

  }
}

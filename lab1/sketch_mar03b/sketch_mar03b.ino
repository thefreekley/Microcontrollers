const int buttonPin = 48; //L1

const byte animationLed[4] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000
};

void setup() {
  DDRA = 0xFF;
  PORTA=0;
  pinMode(buttonPin,INPUT_PULLUP);
}

void loop() {
  if(digitalRead(buttonPin) == LOW){
    for(int i = 0; i<4;i++){
     PORTA = animationLed[i]; 
     delay(900);
    }
    PORTA = 0;
}
}

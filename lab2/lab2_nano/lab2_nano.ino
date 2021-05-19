const int buttonPinFirst = 12;
const int buttonPinSecond = 11;

const byte animationLed[4] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000
};

void setup() {
  DDRD = 0xFF;
  PORTD=0;
  pinMode(buttonPinFirst,INPUT_PULLUP);
  pinMode(buttonPinSecond,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if(!digitalRead(buttonPinFirst))showLedsFirst();
  if(!digitalRead(buttonPinSecond))showLedsSecond();
  if (Serial.available()) {
      char incomingByte = Serial.read();
      int number = incomingByte;
     
     if (number == 1) showLedsFirst();
     else if(number ==  2)showLedsSecond();
  }

}

void showLedsFirst(){
    for(int i = 0; i<4;i++){
     PORTD = animationLed[i]; 
     delay(900);
    }
    PORTD = 0;
}

void showLedsSecond(){
  byte shiftLeds = 0b00000001;
  for (int i = 0; i<8; i++){
    if(shiftLeds == 0) shiftLeds = 0b00000010;
    PORTD = shiftLeds; 
    shiftLeds = shiftLeds << 2;
    delay(900);
  }
  PORTD = 0;
}

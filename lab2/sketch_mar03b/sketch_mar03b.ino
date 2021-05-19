const int buttonPinFirst = 46;
const int buttonPinSecond = 44;

const byte animationLed[4] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000
};

void setup() {
  DDRA = 0xFF;
  PORTA=0;
  pinMode(buttonPinFirst,INPUT_PULLUP);
  pinMode(buttonPinSecond,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if(!digitalRead(buttonPinFirst)){
    Serial.print(1);
//    showLedsFirst();
  }
  if(!digitalRead(buttonPinSecond)){
    Serial.print(2);
//    showLedsSecond();
  }
  if (Serial.available()) {
      char incomingByte = Serial.read();
      int number = incomingByte;
     
     if (number == 1) showLedsFirst();
     else if(number ==  2)showLedsSecond();
  }

}

void showLedsFirst(){
    for(int i = 0; i<4;i++){
     PORTA = animationLed[i]; 
     delay(900);
    }
    PORTA = 0;
}

void showLedsSecond(){
  byte shiftLeds = 0b00000001;
  for (int i = 0; i<8; i++){
    if(shiftLeds == 0) shiftLeds = 0b00000010;
    PORTA = shiftLeds; 
    shiftLeds = shiftLeds << 2;
    delay(900);
  }
  PORTA = 0;
}

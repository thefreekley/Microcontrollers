#define BAUD 14400
#define MODE 2
#define TRANSMIT HIGH
#define RECEIVE LOW
#define ID 77

char symbol;
int i = 0;
char* bufferPtr;



#include "CRC8.h"

#include "CRC.h"


CRC8 crc;

void setup(void) {
  Serial.begin(BAUD);
  pinMode(MODE, OUTPUT);
  digitalWrite(MODE, RECEIVE);
}

void loop() {
  i = 0;
  symbol = 0;
  if (Serial.available()) {
    bufferPtr = (char*)malloc(50 * sizeof(char));
    delay(5);
    while (Serial.available()) {
      symbol = Serial.read();
      if (symbol == ';') {
        bufferPtr[i++] = '\0';
        i = 0;
        int c;
        sscanf(bufferPtr, "%d", &c);
        if (c != ID) {
          while (Serial.available()){Serial.read();}
          free(bufferPtr);
          return;
        }
      } else {
        bufferPtr[i++] = symbol;
      }
    }
    if (i > 0){
      bufferPtr[i++] = '\0';
    }
  }
  if (i > 0) {
    if (!strcmp(bufferPtr, "Give")) {
      char message[] = "Andrii Matviichuk";
      sendMessage(1, message, sizeof(message));
    }
  }
  free(bufferPtr);
  //delay(200);
}

void sendMessage(int recevierId, char str[], long sizeOf) {
  char buffer[20];
  digitalWrite(MODE, TRANSMIT);

  
  long crc = crc8((uint8_t *)str,sizeOf-1,0x7); 
  
  delay(60); 
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer); // Без спотворень
  delay(50);
  str[0] ^= (1<<2);   // 2й розряд найстаршого байту даних Slave1
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  delay(50);
  str[0] ^= (1<<2); //фікс попередньої операції
    
  str[strlen(str) - 1] ^= (1<<5); //5й розряд наймолодшого байту даних Slave2
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  delay(50);
  str[strlen(str) - 1] ^= (1<<5); //фікс попередньої операції
  str[3] ^= (1<<3) | (1<<4);   //3й та 4й розряд 3го байту даних Slave2
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  delay(50);
  str[3] ^= (1<<3) | (1<<4); //фікс попередньої операції

  str[8] ^= (1<<1) | (1<<2) | (1<<6);  //1, 2, 6 розряди 8го байту даних Slave1
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  delay(60);
  digitalWrite(MODE, RECEIVE);
}

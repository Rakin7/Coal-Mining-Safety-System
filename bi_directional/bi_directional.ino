#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int led = 7;

char text[32] = "";

RF24 radio(8,9); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(0, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  radio.startListening();
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&text, sizeof(text));
      Serial.println(text);
    }
    delay(5);
    radio.stopListening();
    radio.write(&text, sizeof(text));
    if(text[0] =='1'){
      digitalWrite(led, HIGH);
      Serial.println("yo yo yo");
    }
    else{
      digitalWrite(led,LOW);
    }
  }
}

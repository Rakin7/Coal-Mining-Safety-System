#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8,9); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  Serial.println("test1");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("test2");
}
void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  Serial.println(text);
  delay(1000);
}

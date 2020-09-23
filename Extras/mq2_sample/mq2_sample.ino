#include <MQ2.h>
  
  int pin = A0;
  
  MQ2 mq2(pin);
  
  void setup(){
    mq2.begin();
    Serial.begin(9600);
  }
 void loop(){

  float* values = mq2.read(true); //true to print the values in the Serial

//  float lpg = mq2.readLPG();
//  
//  float co = mq2.readCO();
//  
//  float smoke = mq2.readSmoke();
  delay(500);
 }

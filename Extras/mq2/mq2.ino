#include <MQ2.h>

int gas_pin = A0;
MQ2 mq2(gas_pin);

void setup() {
  Serial.begin(9600);
  mq2.begin();
}

void loop() {

  float* values = mq2.read(true); //true to print the values in the Serial

//  float lpg = mq2.readLPG();
//  float co = mq2.readCO();
//  float smoke = mq2.readSmoke();

  delay(1000);
}

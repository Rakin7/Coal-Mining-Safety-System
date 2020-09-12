#include "DHT.h"
#include <MQ2.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8,9); // CE, CSN
const byte address[6] = "00001";


String data = "";

//////////////////////////////////// DHT11 ////////////////////
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
//////////////////////////////////// MQ2  /////////////////////
int gas_pin = A0;
MQ2 mq2(gas_pin);
float co;
float smoke;
//////////////////////////////////// fire/////////////////////
int fire = A1;
int fire_value;
///////////////////////////////// panic alarm ////////////////////
int alarm_pin = 4;
boolean alarm = 0;
int buzzer = 3;
int led = 7;
void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  dht.begin();
  mq2.begin();
  pinMode(fire,INPUT);
  pinMode(alarm_pin,INPUT);
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  


}

void loop() {
  data = "";
  h = dht.readHumidity();
  t = dht.readTemperature();
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  fire_value = analogRead(fire);
  if(digitalRead(alarm_pin)==0){
    alarm = 1;
  }
  else if(co>200.0 || fire_value<600 || smoke>200.0 || t>50  ){
     alarm = 1;
  }else{
    alarm = 0;
  }

  if(alarm ==1){
    digitalWrite(led,HIGH);
    digitalWrite(buzzer, HIGH);
    delay(100);
  }else if(alarm ==0){
    digitalWrite(led,LOW);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  
  
  data = data+alarm+","+h+","+t+","+co+","+smoke+","+fire_value;

  char text[50];
  data.toCharArray(text, 50); 
  
  radio.write(&text, sizeof(text));
  Serial.println(text);
  delay(500);
}

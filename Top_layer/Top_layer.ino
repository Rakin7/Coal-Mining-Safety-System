#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>


RF24 radio(2,15); // CE, CSN
const byte address[6] = "00002";


#define led 5 //GPIO 5
#define buzzer 4 //GPIO 4



// Replace with your network credentials
const char* ssid = "TP-Link";
const char* password = "bappy4321";
const char* mqttServer = "dsd1.myftp.org";
const int mqttPort = 8883;
const char* mqttUser = "dsd1";
const char* mqttPassword = "dsd1";
const char* topic_name = "DSD1";


WiFiClient espClient;
PubSubClient client(espClient);



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if ((char)payload[0] == 'P') {
    digitalWrite(led,HIGH);
    digitalWrite(buzzer, HIGH);
    delay(100);
  }
  else if ((char)payload[0] == 'N') {
    digitalWrite(led,LOW);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
}



void setup() {
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(topic_name,mqttUser,mqttPassword)) {
      client.subscribe("panic");
      Serial.println(" connected ");
    } else {
      Serial.print(" failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

  
}


void loop() {
  if (radio.available()) {
    char send_data[50] = "";
    radio.read(&send_data, sizeof(send_data));
    Serial.println(send_data);

    client.publish(topic_name,send_data);
     
  if (send_data[0] == '1') {
    digitalWrite(led,HIGH);
    digitalWrite(buzzer, HIGH);
    delay(100);
  }
//  else {
//    digitalWrite(led,LOW);
//    digitalWrite(buzzer, LOW);
//    delay(100);
//  }
    delay(500);
  }
  client.loop();
}

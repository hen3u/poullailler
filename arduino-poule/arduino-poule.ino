#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

int led_red = 6;
int led_green = 7;
int led_orange = 8;

void setup() {
  Serial.begin(9600);
    
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
  
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_orange, OUTPUT);
}

void loop() {

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    if(strcmp(text,"led_red")==0){
      digitalWrite(led_red, HIGH);
      delay(500);
    } else if(strcmp(text,"led_green")==0){
      digitalWrite(led_green, HIGH);
      delay(500);
    } else if(strcmp(text,"led_orange")==0){
      digitalWrite(led_orange, HIGH);
      delay(500);
    }
  }
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_orange, LOW);
}

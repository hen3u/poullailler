#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

int led_red = 6;
int led_green = 7;
int led_orange = 8;

const char sensor_opened[] = "door_opened";
const char sensor_closed[] = "door_closed";

// Actions
const int NONE=0;
const int CLOSE=1; // red led high
const int OPEN=2;  // green led high
const int REPLY=3; // reply request

//States
const int CLOSED=1;
const int OPENED=2;

int state;
int action;

char text[32] = "";

void setup() {
  Serial.begin(9600);
    
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
  
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_orange, OUTPUT);

  digitalWrite(led_red, HIGH);  // TODO replace with switch value
  digitalWrite(led_green, LOW); // TODO replace with switch value

  state = CLOSED; //TODO replace with switch value
  action = NONE;
  Serial.print("SETUP -->  state=\n");
  Serial.println(state);
}

void loop() {
   
  
   if (radio.available()) {
     radio.read(&text, sizeof(text));
     Serial.print("Reçu: ");
     Serial.print(text);
     Serial.print("\n");
   }
    
   if(strcmp(text,"led_orange")==0){
     action = REPLY;
   } else if(strcmp(text,"led_red")==0){
     action = CLOSE;
   } else if(strcmp(text,"led_green")==0){
     action = OPEN;
   }

   if(action==REPLY){
           Serial.print("REPLY state=\n");
           Serial.println(state);
       radio.stopListening();
       switch(state) {
         case OPENED:
           Serial.print("Envoi opened -->  state=\n");
           Serial.println(state);
           radio.write(&sensor_opened, sizeof(sensor_opened));
           break;
         case CLOSED:
           Serial.print("Envoi closed -->  state=\n");
           Serial.println(state);
           radio.write(&sensor_closed, sizeof(sensor_closed));
           break;
         case NONE:
           Serial.print("NONE -->  state=\n");
           Serial.println(state);
           break;
         
       }
       radio.startListening();
   } else if(action==CLOSE){
       state = CLOSED;
       digitalWrite(led_red, HIGH);
       digitalWrite(led_green, LOW);
   } else if(action==OPEN){
       state = OPENED;
       digitalWrite(led_red, LOW);
       digitalWrite(led_green, HIGH);
   }
   action = NONE;

    /*radio.stopListening();
    radio.write(&sensor_opened, sizeof(sensor_opened));
    radio.startListening();
*/
  /*digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_orange, LOW);*/
}

    
   /* if(strcmp(text,"led_red")==0){
      digitalWrite(led_red, HIGH);
      delay(500);
    } else if(strcmp(text,"led_green")==0){
      digitalWrite(led_green, HIGH);
      delay(500);
    } else if(strcmp(text,"led_orange")==0){
      digitalWrite(led_orange, HIGH);
      delay(500);
    }*/

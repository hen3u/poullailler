#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

/**********************************************************
 *                pinout
 **********************************************************/
const int switchUP = 3;
const int switchDOWN = 4;

/**********************************************************
 *                messages
 **********************************************************/
const char sensor_opened[] = "door_opened";
const char sensor_closed[] = "door_closed";
const char command_stop[] = "door_stop";

/**********************************************************
 *                constants
 **********************************************************/
// Common consts
const int NONE=0;

// Actions consts
const int CLOSE=1; // red led high
const int OPEN=2;  // green led high
const int REPLY=3; // reply request

//States consts
const int CLOSED=1;
const int OPENED=2;

/**********************************************************
 *                variables
 **********************************************************/
int state;
int action;

char text[32] = "";

/**********************************************************
 *                Motor
 **********************************************************/
#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8
#define PWM_MOTOR_1 5
#define CURRENT_SEN_1 A2
#define EN_PIN_1 A0
#define MOTOR_1 0

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;

/**********************************************************
 *                Initialization
 **********************************************************/
void setup() {
  Serial.begin(9600);

  // Initialize motor
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);

  // Initialize radio
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
  
  // Initialize micro-switch
  pinMode(switchUP, INPUT);
  pinMode(switchDOWN, INPUT);

  // Find state value from micro-switch
  if(digitalRead(switchUP) == HIGH)
    state = OPENED;
  else
    state = CLOSED;

  Notify();

  action = NONE;
  Serial.print("SETUP: state=");
  Serial.println(state);
}

/**********************************************************
 *                Main Loop
 **********************************************************/
void loop() {

   digitalWrite(EN_PIN_1, HIGH);
  
   if (radio.available()) {
     radio.read(&text, sizeof(text));
     Serial.print("Reçu: ");
     Serial.println(text);
   }

   if(strcmp(text,"do_notify")==0){
       Notify();
   } else if(strcmp(text,"do_close")==0){
       state = CLOSED;
       Forward();
   } else if(strcmp(text,"do_open")==0){
       state = OPENED;
       Reverse();
   } else if(strcmp(text,"do_stop")==0){
       Stop();
   }

  if((digitalRead(switchUP) == HIGH) &&
    (digitalRead(switchDOWN) == LOW))
    state = OPENED;
  else if((digitalRead(switchUP) == LOW) &&
         (digitalRead(switchDOWN) == HIGH))
    state = CLOSED;
  else
    state = NONE;

   memset(text, 0, sizeof(text));

}

void Notify() {
  switch(state) {
    case OPENED:
      Serial.println("Envoi OPENED");
      radio.stopListening();
      radio.write(&sensor_opened, sizeof(sensor_opened));
      radio.startListening();
      break;
    case CLOSED:
      Serial.println("Envoi CLOSED");
      radio.stopListening();
      radio.write(&sensor_closed, sizeof(sensor_closed));
      radio.startListening();
      break;         
  }     
}

void Stop() {
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
}

void Forward() {
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void Reverse() {
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void IncreaseSpeed() {
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void DecreaseSpeed() {
  usSpeed = usSpeed - 10;
  if(usSpeed < 0) {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) {        //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
  if(motor == MOTOR_1) {
    if(direct == CW) {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW) {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
}

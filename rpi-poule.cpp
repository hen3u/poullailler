#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;

typedef unsigned char byte;
RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);

const byte address[6] = "00001";

const char sensor_red[] = "led_red";
const char sensor_green[] = "led_green";
const char sensor_orange[] = "led_orange";

int main(int argc, char** argv){

  cout << "Starting communication\n";

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.printDetails();

  while (1) {
    radio.write(&sensor_red, sizeof(sensor_red));
    delay(1000);
    radio.write(&sensor_green, sizeof(sensor_green));
    delay(1000);
    radio.write(&sensor_orange, sizeof(sensor_orange));
    delay(1000);
  }

  return 0;
}


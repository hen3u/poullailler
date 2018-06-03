#!/usr/bin/env python

from __future__ import print_function
import time
from RF24 import *

radio = RF24(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ)

address = [0x3130303030] # "00001" (byte)
sensor_red = b'led_red'
sensor_green = b'led_green'
sensor_orange = b'led_orange'

print('Starting communication')
radio.begin()
radio.setPALevel(RF24_PA_LOW);
radio.setDataRate(RF24_250KBPS)
radio.setChannel(108)
radio.openWritingPipe(address[0])
radio.printDetails()

# forever loop
while True:
    radio.write(sensor_red[:len(sensor_red)])
    time.sleep(1)
    radio.write(sensor_green[:len(sensor_green)])
    time.sleep(1)
    radio.write(sensor_orange[:len(sensor_orange)])
    time.sleep(1)

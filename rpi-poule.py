#!/usr/bin/env python

from __future__ import print_function
import time
from RF24 import *
import homeassistant.remote as remote
import yaml

RPI_V2_GPIO_P1_22 = 25
BCM2835_SPI_CS0 = 0
BCM2835_SPI_SPEED_4MHZ = 64

radio = RF24(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ)

address = [0x3130303030] # "00001" (byte)
sensor_red = b'led_red'
sensor_green = b'led_green'
sensor_orange = b'led_orange'

# Read credentials from secrets.yaml
credentials = yaml.load(open('/home/homeassistant/.homeassistant/secrets.yaml'))
password = credentials['http_password']
api = remote.API('127.0.0.1', password)

print(remote.get_config(api))

print('Starting communication')
radio.begin()
radio.setPALevel(RF24_PA_LOW);
radio.setDataRate(RF24_250KBPS)
radio.setChannel(108)
radio.openReadingPipe(0, address[0]);
radio.openWritingPipe(address[0])
radio.printDetails()

init = True

while True:

    if init:
      radio.stopListening()
      radio.write(sensor_orange[:len(sensor_orange)])
      time.sleep(1)
      radio.startListening()
      init = False

    switch_moteur = remote.get_state(api, 'switch.moteur')
    print('{} is {}.'.format(
        switch_moteur.name, switch_moteur.state)
    )
    if switch_moteur.state == "on":
        radio.stopListening()
        radio.write(sensor_red[:len(sensor_red)])
        time.sleep(1)
        radio.startListening()
    elif switch_moteur.state == "off":
        radio.stopListening()
        radio.write(sensor_green[:len(sensor_green)])
        time.sleep(1)
        radio.startListening()
    if radio.available():
        print("radio is available")
        length = radio.getDynamicPayloadSize()
        receive_payload = radio.read(length)
        print(receive_payload)

# forever loop
#while True:
#    radio.write(sensor_red[:len(sensor_red)])
#    time.sleep(1)
#    radio.write(sensor_green[:len(sensor_green)])
#    time.sleep(1)
#    radio.write(sensor_orange[:len(sensor_orange)])
#    time.sleep(1)


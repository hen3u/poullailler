# poullailler

```sh
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade

sudo apt-get install git
sudo apt-get install vim

sudo apt-get install python3-dev
sudo apt-get install libboost-python-dev
sudo ln -s /usr/lib/arm-linux-gnueabihf/libboost_python-py35.so /usr/lib/arm-linux-gnueabihf/libboost_python3.so
sudo apt-get install python3-setuptools

sudo apt-get install python3 python3-venv python3-pip
sudo useradd -rm homeassistant -G dialout

cd /srv
sudo mkdir homeassistant
sudo chown homeassistant:homeassistant homeassistant

sudo su -s /bin/bash homeassistant
cd /srv/homeassistant
python3 -m venv .
source bin/activate

(homeassistant) homeassistant@raspberrypi:/srv/homeassistant $ python3 -m pip install wheel
(homeassistant) homeassistant@raspberrypi:/srv/homeassistant $ pip3 install homeassistant
(homeassistant) $ hass

CTRL+D

sudo vim /etc/systemd/system/home-assistant@homeassistant.service
```
```
[Unit]
Description=Home Assistant
After=network-online.target

[Service]
Type=simple
User=%i
ExecStart=/srv/homeassistant/bin/hass -c "/home/homeassistant/.homeassistant"

[Install]
WantedBy=multi-user.target
```
```sh
sudo systemctl --system daemon-reload
sudo systemctl enable home-assistant@homeassistant
sudo systemctl start home-assistant@homeassistant

sudo raspi-config   --> Interfacing Options --> SPI --> OK
ls /dev/spidev* --> check that spidev is present


sudo su -s /bin/bash homeassistant
. /srv/homeassistant/bin/activate

git clone https://github.com/nRF24/RF24.git
cd RF24/
./configure --prefix=$HOME/rf24 --soc=BCM2835 --c_compiler=arm-linux-gnueabihf-gcc --cxx_compiler=arm-linux-gnueabihf-g++ --driver=SPIDEV --ldconfig=''
make
make install
cd pyRF24/
python3 ./setup.py build
python3 ./setup.py install

git clone https://github.com/hen3u/poullailler.git
cd poullailler/
export LD_LIBRARY_PATH=$HOME/rf24/lib
```
Add in rpi-poule.py :
RPI_V2_GPIO_P1_22 = 25
BCM2835_SPI_CS0 = 0
BCM2835_SPI_SPEED_4MHZ = 64

```sh
python3 rpi-poule.py
```


#### TODO ####
1) Automatisation:
export LD_LIBRARY_PATH=/home/homeassistant/rf24/lib
. /srv/homeassistant/bin/activate
python3 ./rpi-poule.py
2) Home assistant components
3) Python Api integration
4) Motor driver on arduino side

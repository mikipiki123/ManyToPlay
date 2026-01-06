
# Installation:

(MacOS) brew install sdl2 <br>
(RPI) sudo apt install libsdl2-dev

#### from root dir use the installer.

chmod +x install.sh

./install.sh

#### register:
venv/bin/python -m pyremoteplay [PS IPv4]

After the user register to pyremoteplay connect the controller:

controller on pairing mode + python connect.py

#### if something wrong write: 

sudo rfkill unblock bluetooth

#### and finally to play:

venv/bin/python -m pyremoteplay [PS IPv4]


#### notes
1. the keyboard design for test and debug, only base controls implemented.
2. install.sh works only for linux enviroment.

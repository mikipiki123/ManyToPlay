
# Installation:

(MacOS) brew install sdl2 <br>
(RPI) sudo apt install libsdl2-dev

cd controller <br>
mkdir build && cd build <br>
cmake .. <br>
make <br>

mkfifo /tmp/my_pipe

#### from root dir:
python -m venv venv

source venv/bin/activate <br>
pip install -r requirements.txt <br>
venv/bin/python -m pyremoteplay [PS IPv4]


#### notes
note that the keyboard design for test and debug, only base controls implemented.

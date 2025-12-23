
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


#### Known issue
delay when buttons pressed simultaneously with R2 or L2

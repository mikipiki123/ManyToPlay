#!/bin/bash
set -e

PACKAGES=(cmake gcc g++ make libsdl2-dev)

sudo apt update

for pkg in "${PACKAGES[@]}"; do
    sudo apt install -y "$pkg"
done


if [ ! -d "./controller/build" ]; then
	echo "compiling the controller hanlder.."
	cd controller
	mkdir build && cd build
	sleep 1
	cmake ..

	sleep 2
	make
	sleep 2
	cd ../..

else
    echo "build dir already exists."
fi


PIPE="/tmp/my_pipe"

if [ ! -p "$PIPE" ]; then
    echo "Pipe does not exist. Creating it..."
    mkfifo /tmp/my_pipe
else
    echo "Pipe already exists."
fi


python -m venv venv

source venv/bin/activate
pip install -r requirements.txt

sudo rfkill unblock bluetooth

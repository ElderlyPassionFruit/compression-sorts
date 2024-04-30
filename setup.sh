#!/bin/bash
sudo apt update
sudo apt upgrade -y

sudo apt install python3 python3-dev git wget unzip acl build-essential libssl-dev libffi-dev liblz4-dev -y

rm -rf build/ 
mkdir build/
cd build
cmake ..
make -j
cd ..

python3 tests_data/generate_tests_data.py

#!/bin/bash
sudo apt update
sudo apt upgrade -y

sudo apt install python3 python3-dev git wget unzip acl build-essential libssl-dev libffi-dev liblz4-dev linux-tools-common linux-tools-generic -y

GTEST_VERSION=1.12.1

rm -rf googletest
git clone https://github.com/google/googletest.git -b release-${GTEST_VERSION} \
    && cd googletest \
    && mkdir build \
    && cd build \
    && cmake .. \
    && sudo make install -j \
    && cd .. \
    && cd .. \
    && rm -rf googletest

rm -rf build/ 
mkdir build/
cd build
cmake ..
make -j
cd ..

./generator/download_data.sh
# python3 generator/download_data.py
./bin/generate_tests_data

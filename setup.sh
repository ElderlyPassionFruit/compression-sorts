#!/bin/bash

# install linux dependencies
sudo apt update
sudo apt upgrade -y
sudo apt install python3 python3-dev git wget unzip acl build-essential libssl-dev libffi-dev liblz4-dev linux-tools-common linux-tools-generic -y

# install python dependencies into the virtual environment
python3 -m venv --without-pip .venv
source /.venv/bin/activate
curl https://bootstrap.pypa.io/get-pip.py | python3
deactivate
source /.venv/bin/activate
pip install --upgrade -r requirements.txt

# install googletests with fixed release version
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

# build all targets
rm -rf build/ 
mkdir build/
cd build
cmake ..
make -j
cd ..

# download and prepare datasets
./generator/download_data.sh

# generate test batches
./bin/generate_tests_data

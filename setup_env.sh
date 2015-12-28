#!/bin/sh

git clone https://github.com/mbedmicro/mbed.git

cd mbed
git checkout f6adbf74dcb911e6f68ecb793ca762bd7be444a0
git apply ../patch_mbed_for_empirikit.diff

# Pull gcc arm
curl --location https://launchpad.net/gcc-arm-embedded/4.7/4.7-2012-q4-major/+download/gcc-arm-none-eabi-4_7-2012q4-20121208-linux.tar.bz2 | tar jx
ln -s gcc-arm-none-eabi-4_7-2012q4 gcc-arm

cd ..

# Python deps (uncomment or run directly)
# sudo pip install Jinja2
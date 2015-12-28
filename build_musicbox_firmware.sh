#!/bin/sh

ROOT_DIR=`pwd`

rm $ROOT_DIR/musicbox/*.o
rm $ROOT_DIR/musicbox/*.d
rm $ROOT_DIR/musicbox/*.elf
rm $ROOT_DIR/musicbox/*.bin
cd mbed
./workspace_tools/make.py --usb --peripherals=TSI,MMA8451Q -m KL25Z -t GCC_ARM --source $ROOT_DIR/musicbox

cd $ROOT_DIR
cp $ROOT_DIR/musicbox/musicbox.bin ./musicbox.bin


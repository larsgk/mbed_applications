#!/bin/sh

ROOT_DIR=`pwd`

rm $ROOT_DIR/xmasgame/*.o
rm $ROOT_DIR/xmasgame/*.d
rm $ROOT_DIR/xmasgame/*.elf
rm $ROOT_DIR/xmasgame/*.bin
cd mbed
./workspace_tools/make.py --usb --peripherals=TSI,MMA8451Q -m KL25Z -t GCC_ARM --source $ROOT_DIR/xmasgame

cd $ROOT_DIR
cp $ROOT_DIR/xmasgame/xmasgame.bin ./xmasgame.bin


#!/bin/sh

ROOT_DIR=`pwd`

rm $ROOT_DIR/empirikit/*.o
rm $ROOT_DIR/empirikit/*.d
rm $ROOT_DIR/empirikit/*.elf
rm $ROOT_DIR/empirikit/*.bin
cd mbed
./workspace_tools/make.py --usb --peripherals=TSI,MMA8451Q -m KL25Z -t GCC_ARM --source $ROOT_DIR/empirikit

cd $ROOT_DIR
cp $ROOT_DIR/empirikit/empirikit.bin ./empiriKitMOTION.bin


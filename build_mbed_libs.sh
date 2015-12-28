#!/bin/sh

cd mbed && python workspace_tools/build.py -m KL25Z -t GCC_ARM -u && cd ..
cd mbed && python workspace_tools/build.py -m KL46Z -t GCC_ARM -u && cd ..


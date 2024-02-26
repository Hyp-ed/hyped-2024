#!/bin/bash

set -eu

# Fix for git
git config --global --add safe.directory '*'

cd $DIR

if [[ -d "build" && $CLEAN = true ]]; then
    rm -r build  
    mkdir build
elif [ ! -d "build" ]; then
    mkdir build
fi

if [[ $CROSS_COMPILE = true ]]; then
    echo "Cross compiling for Raspberry Pi..."
    cd build 
    cmake  -DCMAKE_CXX_COMPILER=aarch64-rpi3-linux-gnu-g++ -DCURSES_LIBRARY=/usr/lib/x86_64-linux-gnu/libncurses.so -DCURSES_INCLUDE_PATH=/usr/include ..
    make -j
else
    echo "Building..."
    cd build 
    cmake ..
    make -j
    make test
fi 

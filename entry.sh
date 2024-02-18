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
    cmake .. #TODO add cross compile flags
    make -j
    make test
else
    echo "Building..."
    cd build 
    cmake ..
    make -j
    make test
fi 

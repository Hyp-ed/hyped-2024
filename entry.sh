#!/bin/bash

set -eu

echo "Building..."

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
    cd build 
    cmake ..
    make -j
    make test
else
    cd build 
    cmake ..
    make -j
    make test
fi 

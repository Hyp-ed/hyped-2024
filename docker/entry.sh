#!/bin/bash

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

cd build 
cmake ..
make -j
make test

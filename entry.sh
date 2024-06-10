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

cd build 
if [[ $LINT = true ]]; then
    cmake .. -DLINT=ON
else
    cmake ..
fi

make -j$(nproc)
make test

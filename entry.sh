#!/bin/bash
echo "$CLEAN"
echo "$CROSS_COMPILE"

echo "Building..."

cd /home/hyped_build

if [ -d "build" ]; then
    if [ $CLEAN = true ]; then
        rm -r build  
        mkdir build
    fi    
else
    mkdir build
fi


cd build 
cmake ..
make -j
#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <yarn_script>"
  exit 1
fi

export YARN_SCRIPT=$1

echo "Running with yarn script: $YARN_SCRIPT"

if [ "$2" == "--build" ]; then
  echo "Building docker image"
  docker-compose up --build
else
  echo "Using cached docker image"
  docker-compose up
fi

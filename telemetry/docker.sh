#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <yarn_script>"
  exit 1
fi

export YARN_SCRIPT=$1

echo "Running with yarn script: $YARN_SCRIPT"

docker-compose up --build

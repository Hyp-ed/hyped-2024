#!/bin/bash

# Adapted from: https://medium.com/@wujido20/handling-flags-in-bash-scripts-4b06b4d0ed04

# Function to display script usage
usage() {
 echo "Usage: $0 <yarn_script> [OPTIONS]"
 echo "Options:"
 echo " -h, --help              Display this help message"
 echo " -b, --build             Build the docker image before running the container"
 echo " -m, --with-mqtt-broker  Run the MQTT broker along with the container"
}

# Default values for options
build=false
with_mqtt_broker=false

if [ -z "$1" ]; then
  echo "Missing argument: yarn_script" >&2
  usage
  exit 1
fi

export YARN_SCRIPT=$1
shift

# Function to handle options and arguments
handle_options() {
  while [ $# -gt 0 ]; do
    case $1 in
      -h | --help)
        usage
        exit 0
        ;;
      -b | --build)
        build=true
        ;;
      -m | --with-mqtt-broker)
        with_mqtt_broker=true
        ;;
      *)
        echo "Invalid option: $1" >&2
        usage
        exit 1
        ;;
    esac
    shift
  done
}

# Main script execution
handle_options "$@"

if [ "$build" = true ]; then
  if [ "$with_mqtt_broker" = true ]; then
    docker-compose -f docker-compose.yml -f docker-compose.mqtt.yml up --build
  else
    docker-compose up --build
  fi
elif [ "$with_mqtt_broker" = true ]; then
  docker-compose -f docker-compose.yml -f docker-compose.mqtt.yml up
else
  docker-compose up
fi

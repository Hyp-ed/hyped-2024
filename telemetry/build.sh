#!/bin/bash

IMAGE_NAME=telemetry_build
CONTAINER_NAME=telemetry_build

# Adapted from: https://medium.com/@wujido20/handling-flags-in-bash-scripts-4b06b4d0ed04

# Function to display script usage
usage() {
 echo "Usage: $0 [OPTIONS]"
 echo "Options:"
 echo " -h, --help              Display this help message"
 echo " -b, --build             Build the docker image before running the container"
}

# From Kshitij
if ! [ -x "$(command -v docker)" ]; then
  echo '[!] Error: docker is not installed.' >&2
  exit 1
fi

image=$( docker images -q $IMAGE_NAME 2> /dev/null )
if [[ -z ${image} ]]; then
  echo "[!] Building image"
  docker build -t $IMAGE_NAME .
else 
  echo "[>] Image already built"
fi

# Default values for options
build=false
with_mqtt_broker=false

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
    echo "Building docker image..."
    docker build -t $IMAGE_NAME .
fi

# Check if the container name already exists
container=$( docker ps -a -q --filter name=$CONTAINER_NAME 2> /dev/null )

# If the container exists, remove it
if [[ -n ${container} ]]; then
  echo "[!] Found existing container. Removing container"
  docker rm $CONTAINER_NAME
fi

MSYS_NO_PATHCONV=1 docker run -e PNPM_SCRIPT=build --name $CONTAINER_NAME -v $(pwd):/usr/src/app \
  -v /usr/src/app/node_modules \
  -v /usr/src/app/packages/server/node_modules \
  -v /usr/src/app/packages/ui/node_modules \
  -v /usr/src/app/packages/constants/node_modules \
  -v /usr/src/app/packages/types/node_modules \
  -v /usr/src/app/packages/public-app/node_modules \
  -v /usr/src/app/packages/public-app/.next \
  $IMAGE_NAME
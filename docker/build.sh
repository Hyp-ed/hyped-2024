#!/bin/bash

IMAGE_NAME="hyped_build"
CONTAINER_NAME="hyped_build"

# Adapted from: https://medium.com/@wujido20/handling-flags-in-bash-scripts-4b06b4d0ed04

# Function to display script usage
usage() {
 echo "Usage: $0 <yarn_script> [OPTIONS]"
 echo "Options:"
 echo " -h,  --help              Display this help message"
 echo " -r,  --rebuild           Rebuild the docker image before running the container"
 echo " -c,  --clean             Makes a clean build directory"
 echo " -cc, --cross-compile     Cross-compile for Raspberry Pi"
}

# From Kshitij
if ! [ -x "$(command -v docker)" ]; then
  echo '[!] Error: docker is not installed.' >&2
  exit 1
fi

if ! [ -x "$(command -v docker-compose)" ]; then
  echo '[!] Error: docker-compose is not installed.' >&2
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
rebuild=false
clean=false
cross_compile=false

# Function to handle options and arguments
handle_options() {
  while [ $# -gt 0 ]; do
    case $1 in
      -h | --help)
        usage
        exit 0
        ;;
      -r | --rebuild)
        rebuild=true
        ;;
      -c | --clean)
        clean=true
        ;;
      -cc | --cross-compile)
        cross_compile=true
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

if [ "$rebuild" = true ]; then
    echo "Rebuild"
    docker build . -t $IMAGE_NAME
fi

# Check if the container name already exists
container=$( docker ps -a -q --filter name=$CONTAINER_NAME 2> /dev/null )

# If the container exists, remove it
if [[ -n ${container} ]]; then
  echo "[!] Found existing container. Removing container"
  docker rm $CONTAINER_NAME
fi

cd ..
docker run -e CLEAN=$clean -e CROSS_COMPILE=$cross_compile -e DIR=/home/$IMAGE_NAME --name $CONTAINER_NAME -v $(pwd):/home/$IMAGE_NAME $IMAGE_NAME bash

#!/bin/bash

set -eu

IMAGE_NAME="hyped"
BUILD_CONTAINER_NAME="hyped_build"
DEV_CONTAINER_NAME="hyped_dev"

# Adapted from: https://medium.com/@wujido20/handling-flags-in-bash-scripts-4b06b4d0ed04

# Function to display script usage
usage() {
 echo "Usage: $0 <yarn_script> [OPTIONS]"
 echo "Options:"
 echo " -b,  --build             Build with container"
 echo " -d,  --dev               Run development container"
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

# Check if image is built
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
docker_build=false
docker_dev=false

# Function to handle options and arguments
handle_options() {
  while [ $# -gt 0 ]; do
    case $1 in
      -b | --build)
        docker_build=true
        ;;
      -d | --dev)
        docker_dev=true
        ;;
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
  docker build -t $IMAGE_NAME .
fi

if [ "$docker_build" = true ]; then
  # Check if the container name already exists
  build_container=$( docker ps -a -q --filter name=$BUILD_CONTAINER_NAME 2> /dev/null )

  # If the container exists, remove it
  if [[ -n ${build_container} ]]; then
    echo "[!] Found existing container. Removing container"
    docker rm $BUILD_CONTAINER_NAME
  fi

  docker run -e CLEAN=$clean -e CROSS_COMPILE=$cross_compile -e DIR=/home/hyped --name $BUILD_CONTAINER_NAME -v $(pwd):/home/hyped $IMAGE_NAME bash
fi

if [ "$docker_dev" = true ]; then
  # Check if the container name already exists
  dev_container=$( docker ps -a -q --filter name=$DEV_CONTAINER_NAME 2> /dev/null )

  if [[ -n ${dev_container} ]]; then
    echo "[>] Found existing container"
    docker start -i $DEV_CONTAINER_NAME
  else
    echo "[!] No existing container found. Creating new container"
    docker run -it -v $(pwd):/home/hyped -e CLEAN=$clean -e CROSS_COMPILE=$cross_compile --name $DEV_CONTAINER_NAME -w /home/hyped/ --entrypoint /bin/bash $IMAGE_NAME 
  fi
fi
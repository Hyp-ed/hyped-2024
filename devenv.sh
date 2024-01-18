#!/bin/bash

IMAGE_NAME="hyped_build"
CONTAINER_NAME="hyped_dev"

container=$( docker ps -a -q --filter name=$CONTAINER_NAME 2> /dev/null )

if [[ -n ${container} ]]; then
  echo "[!] Found existing container"
  docker start -i $CONTAINER_NAME
else
  docker run -it --mount type=bind,source=$(pwd),target=/home/hyped/ --name $CONTAINER_NAME --entrypoint /bin/bash $IMAGE_NAME
fi

#!/bin/bash

# Use docker env files
cp /usr/src/app/packages/server/.env.docker /usr/src/app/packages/server/.env

cp /usr/src/app/packages/ui/.env.docker /usr/src/app/packages/ui/.env

yarn build

if [ "$YARN_SCRIPT" = "build" ]; then
  echo "Build complete"
  exit 0
fi

yarn "$YARN_SCRIPT"

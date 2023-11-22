#!/bin/bash

# Use docker env files
cp /usr/src/app/server/.env.docker /usr/src/app/server/.env
cp /usr/src/app/ui/.env.docker /usr/src/app/ui/.env

yarn build

yarn "$YARN_SCRIPT"

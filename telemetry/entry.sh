#!/bin/bash

# Use docker env files
cp /usr/src/app/packages/server/.env.docker /usr/src/app/packages/server/.env

cp /usr/src/app/packages/ui/.env.docker /usr/src/app/packages/ui/.env

pnpm build

if [ "$PNPM_SCRIPT" = "build" ]; then
  echo "Build complete"
  exit 0
fi

pnpm "$PNPM_SCRIPT"

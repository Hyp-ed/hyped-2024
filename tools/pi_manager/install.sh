#!/usr/bin/bash

# Must be run as root
if [ "$EUID" -ne 0 ]
  then echo "This script must be run as root"
  exit 1
fi

# Install the pi_manager service
cp pi_manager.service /etc/systemd/system/
cp pi_manager /usr/local/bin/
systemctl enable pi_manager
systemctl start pi_manager
echo "pi_manager service installed and started"

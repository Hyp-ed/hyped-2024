#!/usr/bin/bash

# Must be run as root
if [ "$EUID" -ne 0 ]
  then echo "This script must be run as root"
  exit 1
fi

# Install the pi_manager service
rm /etc/systemd/system/pi_manager.service
cp pi_manager.service /etc/systemd/system/
rm /usr/bin/pi_manager
cp pi_manager /usr/bin/
chmod +x /usr/bin/pi_manager
chown root:root /usr/bin/pi_manager
chown root:root /etc/systemd/system/pi_manager.service
systemctl daemon-reload
systemctl enable pi_manager
systemctl start pi_manager
echo "pi_manager service installed and started"

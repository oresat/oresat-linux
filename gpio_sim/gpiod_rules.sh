#!/bin/bash
sudo groupadd gpiod
sudo usermod -aG gpiod $USER

# This gives user permissions for all gpio.
# Might want to change it to specific pins?

echo '# udev rules for gpio port access through libgpiod
SUBSYSTEM=="gpio", KERNEL=="gpiochip[0-4]", GROUP="gpiod", MODE="0660"' | sudo tee /etc/udev/rules.d/60-gpiod.rules

#!/bin/bash

# Check if the script is running as root
if [ "$EUID" -ne 0 ]; then
  echo "This script must be run as root. Re-running with sudo..."
  sudo "$0" "$@"
  exit
fi

# Edit the config.txt file to remove the RTC overlay
CONFIG_FILE="/boot/firmware/config.txt"
RTC_OVERLAY="dtoverlay=i2c-rtc-gpio,ds3231,bus=2,i2c_gpio_sda=13,i2c_gpio_scl=6"

echo "Removing RTC overlay from $CONFIG_FILE..."
if grep -q "$RTC_OVERLAY" "$CONFIG_FILE"; then
    sed -i "\|$RTC_OVERLAY|d" "$CONFIG_FILE"
    echo "RTC overlay removed."
else
    echo "RTC overlay not found in $CONFIG_FILE."
fi

# Reinstall fake-hwclock and restore its service
echo "Reinstalling fake-hwclock and restoring its service..."
apt update
apt -y install fake-hwclock
update-rc.d fake-hwclock defaults

# Uncomment the lines in hwclock-set script
HW_CLOCK_SET_FILE="/lib/udev/hwclock-set"
echo "Restoring $HW_CLOCK_SET_FILE..."
sed -i 's/^#if \[ -e \/run\/systemd\/system \] ; then$/if [ -e \/run\/systemd\/system ] ; then/' "$HW_CLOCK_SET_FILE"
sed -i 's/^#    exit 0$/    exit 0/' "$HW_CLOCK_SET_FILE"
echo "Restoration complete."

echo "RTC removal script completed."

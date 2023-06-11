Octavo A8 Boot Process
======================

- The AM335x's ``LCD_DATA2`` pin, aka Octavo's ``G1`` pin, is the boot select
  pin. On power the AM335x will check so see if the this input. If set high
  (3.3v) the AM335x will try to boot off the eMMC, if set low the AM335x
  will try to boot off the SD card.
- The AM335x will then boot U-Boot image of the select storage device.
- U-Boot will read the AM335x's EEPROM to figure out what specific AM335x
  device it is. OreSat cards will have the Beagleboard's PocketBeagle value
  wriiten to its EEPROM, so U-Boot will then load the PocketBeagle's device
  tree.
- U-Boot will then try to boot the Linux kernel and will pass of the device
  tree to the Linux kernel.
- The Linux kernel will apply any device tree overlays.
- Systemd starts up.
- If DKMS is installed, DKMS loads any external kernel modules.
- Systemd starts the card's OLAF app.
- CAN bus is relase by the kernel. OLAF configures the CAN bus and starts sending
  out CANopen heartbeats.
- PRUs are available (if configured in device tree).

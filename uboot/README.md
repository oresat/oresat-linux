# U-Boot

## Setup 

Install dependencies

```bash
sudo pacman -S arm-none-eabi-gcc
```

Clone U-Boot

```bash
$ git clone https://github.com/beagleboard/u-boot
```

## Build

Make the config

```bash
$ make ARCH=arm CROSS_COMPILE=arm-none-eabi- am335x_evm_defconfig
```

Compile

```bash
$ make ARCH=arm CROSS_COMPILE=arm-none-eabi-
```

## Flash

Flash img and MLO

```bash
$ sudo dd if=u-boot-dtb.img of=/dev/sda count=4 seek=1 bs=384k
$ sudo dd if=MLO of=/dev/sda count=2 seek=1 bs=128k
```

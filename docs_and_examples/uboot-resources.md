# Uboot notes

## Making u-boot files for beaglebone on host machine running Arch
- Get arm compiler
    - `pacman -S arm-none-eabi-gdb arm-none-eabi-gcc arm-none-eabi-binutils`
- Get uboot
    - `git clone https://github.com/u-boot/u-boot.git`
- Make MLO and u-boot.img
    - `cd u-boot`
    - `make am335x_evm_defconfig`
    - `ARCH=arm CROSS_COMPILE=arm-none-eabi- make`

## Making u-boot files for beaglebone on host machine running Debain
- Get arm compiler
    - `apt install gcc-arm-linux-gnueabihf`
- Get uboot
    - `git clone https://github.com/u-boot/u-boot.git`
- Make MLO and u-boot.img
    - `cd u-boot`
    - `make am335x_evm_defconfig`
    - `ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- make`

## Links for U-Boot:
- Because BBB is arm based, to learn what arm expects from boot loader: [Link](https://wiki.osdev.org/ARM_Overview)
- Crash course on boot loaders: [Link](https://manybutfinite.com/post/kernel-boot-process/)
- This guy does a lot of examples and such for low level programming/bootloaders: [Link](https://github.com/cirosantilli/x86-bare-metal-examples)
- Resource that OSDev.org refers to read often: [Link](https://software.intel.com/en-us/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d-and-4)
- Overall goto resource: [Link](https://wiki.osdev.org/Expanded_Main_Page)

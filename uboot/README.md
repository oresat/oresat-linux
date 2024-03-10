# U-Boot

## Setup

Install the GCC cross compiler for ARM Cortex-R/M processors

- Arch Linux
  ```bash
  sudo pacman -S arm-none-eabi-gcc openssl bc
  ```
- Debian Linux
  ```bash
  sudo apt install gcc-arm-none-eabi libssl-dev bc
  ```

## Build

This will generate the `u-boot.img` and `MLO`.

```bash
$ make
```

## Flash

Flash `u-boot.img` and `MLO`. Change `/dev/sda` as needed.

```bash
$ sudo dd if=u-boot-dtb.img of=/dev/sda count=4 seek=1 bs=384k
$ sudo dd if=MLO of=/dev/sda count=2 seek=1 bs=128k
```

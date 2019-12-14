# Device Tree
ARM uses a device tree to set hardware descriptions like memory sizing, pin muxing, interupts configurations, and more. It is recommend to let the boot loader handle loading the device tree. If the Beaglebone_installing_arch.md was followed, /boot/dtbs/am335x-boneblack.dtb will be loaded by u-boot.

## Modify the Device tree
Device tree on mainline Linux are not dynamic, meaning it's not able to change or be patched on runtime or by userspace, but it can be recompiled as needed.
- Install device tree compiler
    - `sudo apt-get install device-tree-compiler` on Debian or `sudo pacman -S dtc` on Arch
- Backup device tree binary
    - `cp devicetree_file_name.dtb devicetree_file_name.dtb.bak`
- Compile dts into dtb
    - `dtc -I dts -O dtb -f devicetree_file_name.dts -o devicetree_file_name.dtb`
- Modify device tree source file
- Disassemble dtb into dts
    - `dtc -I dtb -O dts -f devicetree_file_name.dtb -o devicetree_file_name.dts`

## BeagleBoards and capemgr
BeagleBoard Linux images have capemgr. Capemgr allows for dynamic device tree, but it limited to kernel they modified to support it. It is done by device tree overlays. As the name implies, these are device tree binary overlay are just applied ontop of the actual device tree.

## Useful Links
[Device Tree Specs](https://www.devicetree.org/specifications/)
[Device Tree Reference](https://elinux.org/Device_Tree_Reference)
[TI pin mux tool](http://www.ti.com/tool/PINMUXTOOL)

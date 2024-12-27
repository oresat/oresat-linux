CONFIG ?= am335x_oresat.config
boards = c3 cfc dev dxwifi generic gps star-tracker

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS =
else
	FLAGS = ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

.PHONY: all clean $(boards) device_trees

all: c3

$(boards): build_images.sh device_trees u-boot/MLO u-boot/u-boot-dtb.img | image-builder/readme.md
	./build_images.sh $@

device_trees:
	make -C ../device_trees

u-boot/MLO u-boot/u-boot-dtb.img &: $(CONFIG) | u-boot/README
	KCONFIG_CONFIG=../$(CONFIG) $(FLAGS) make -C u-boot

u-boot/README image-builder/readme.md:
	$(error $(dir $@) not initialized. Hint: git submodule update --init)

clean:
	rm -f image_builder/configs/oresat*
	rm -f image_builder/target/chroot/oresat*
	rm -f image_builder/tools/hwpack/oresat-bootloader.conf
	make mrproper -C u-boot
	make clean -C ../device_trees

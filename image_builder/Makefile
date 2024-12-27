UBOOT_REPO= u-boot
IMAGE_BUILDER_REPO= image-builder
DEVICE_TREES= ../device_trees
CONFIG= ../../am335x_oresat_defconfig
BOOTLOADER_DIR= oresat_bootloader
PATCH_FILE= oresat_extlinux.patch
TARGET_FILE= image-builder/tools/setup_sdcard.sh

.PHONY: all image_builder uboot config clone clean

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS=
else
	FLAGS= ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

all: clone config uboot

image_builder: uboot 
	./build_images.sh $(board)

uboot: config
	mkdir -p $(BOOTLOADER_DIR);
	@if [ ! -f "$(BOOTLOADER_DIR)/MLO" ] || \
			[ ! -f "$(BOOTLOADER_DIR)/u-boot-dtb.img" ]; then \
		make -C $(UBOOT_REPO) $(FLAGS); \
		mv $(UBOOT_REPO)/MLO $(BOOTLOADER_DIR); \
		mv $(UBOOT_REPO)/*.img $(BOOTLOADER_DIR); \
	fi

config: clone
	make -C $(UBOOT_REPO) $(FLAGS) $(CONFIG)

clone:
	@if [ -z "$( ls -A $(IMAGE_BUILDER_REPO) )" ] || \
			[ -z "$( ls -A $(UBOOT_REPO) )" ]; then \
		git submodule update --init; \
		if patch -N -i $(PATCH_FILE) $(TARGET_FILE) > /dev/null 2>&1; then \
			echo "$(TARGET_FILE) patched!"; \
		fi \
	fi

clean:
	rm -rf $(BOOTLOADER_DIR)/*
	rm $(IMAGE_BUILDER_REPO)/configs/oresat*
	rm $(IMAGE_BUILDER_REPO)/target/chroot/oresat*
	rm $(IMAGE_BUILDER_REPO)/tools/setup_sdcard.sh.rej
	make clean -C $(UBOOT_REPO)

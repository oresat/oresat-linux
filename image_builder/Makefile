UBOOT_REPO= u-boot
IMAGE_BUILDER_REPO= image-builder
DEVICE_TREES= ../device_trees
CONFIG= am335x_oresat.config
BOOTLOADER_DIR= oresat_bootloader

.PHONY: all uboot device_trees test clean

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS=
else
	FLAGS= ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

all: test uboot
	./build_images.sh $(board)

uboot: device_trees
	mkdir -p $(BOOTLOADER_DIR)
	@if [ ! -f "$(BOOTLOADER_DIR)/MLO" ] || \
			[ ! -f "$(BOOTLOADER_DIR)/u-boot-dtb.img" ]; then \
		cp $(CONFIG) $(UBOOT_REPO)/.config; \
		make -C $(UBOOT_REPO) $(FLAGS); \
		mv $(UBOOT_REPO)/MLO $(BOOTLOADER_DIR); \
		mv $(UBOOT_REPO)/*.img $(BOOTLOADER_DIR); \
	fi

device_trees:
	make -C $(DEVICE_TREES)

test:
ifeq ($(wildcard $(UBOOT_REPO)/*)$(wildcard $(IMAGE_BUILDER_REPO)/*),)
	@echo "Build failed, missing dependency..."
	@echo "Hint: git submodule update --init"
	@exit 1 
endif

clean:
	rm -rf $(BOOTLOADER_DIR)/*
	rm -f $(IMAGE_BUILDER_REPO)/configs/oresat*
	rm -f $(IMAGE_BUILDER_REPO)/target/chroot/oresat*
	rm -f $(IMAGE_BUILDER_REPO)/tools/setup_sdcard.sh.rej
	rm -f $(UBOOT_REPO)/$(CONFIG)
	make clean -C $(UBOOT_REPO)
	make clean -C $(DEVICE_TREES)

UBOOT_REPO= u-boot
IMAGE_BUILDER_REPO= image-builder
DEVICE_TREES= ../device_trees
CONFIG= am335x_oresat.config
list= c3 cfc dev dxwifi generic gps star-tracker

.PHONY: all uboot device_trees test clean board $(list)

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS=
else
	FLAGS= ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

all: uboot
	./build_images.sh $(board)

uboot: test device_trees
	@if [ ! -f "$(UBOOT_REPO)/MLO" ] || \
			[ ! -f "$(UBOOT_REPO)/u-boot-dtb.img" ]; then \
		make -C u-boot mrproper; \
		KCONFIG_CONFIG=../$(CONFIG) $(FLAGS) make -C $(UBOOT_REPO); \
	fi

device_trees:
	make -C $(DEVICE_TREES)

test:
	@if [ -z "$(wildcard $(UBOOT_REPO)/*)" ] || \
			[ -z "$(wildcard $(IMAGE_BUILDER_REPO)/*)" ]; then \
		echo "Hint: git submodule update --init"; \
		exit 1; \
  fi

clean:
	rm -f $(IMAGE_BUILDER_REPO)/configs/oresat*
	rm -f $(IMAGE_BUILDER_REPO)/target/chroot/oresat*
	rm -f $(IMAGE_BUILDER_REPO)/tools/setup_sdcard.sh.rej
	rm -f $(IMAGE_BUILDER_REPO)/tools/hwpack/oresat.conf
	make clean -C $(UBOOT_REPO)
	make clean -C $(DEVICE_TREES)

REPO= u-boot
DEVICE_TREE= ../device_trees
CONFIG= am335x_oresat_defconfig

.PHONY: all clean

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS=
else
	FLAGS= ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

all: patch
	make -C $(DEVICE_TREE)
	@cp $(wildcard $(DEVICE_TREE)/*.dtb) u-boot/arch/arm/dts/
	make -C $(REPO) $(FLAGS) $(CONFIG)
	make -C $(REPO) $(FLAGS)
	@cp $(REPO)/MLO .
	@cp $(REPO)/*.img .

patch: clone
	@cp $(wildcard am335x_*_defconfig) $(REPO)/configs/
	@patch -p1 < u-boot.patch

clone:
	@if [ ! -d $(REPO) ]; then \
		git clone https://github.com/beagleboard/$(REPO); \
	fi

clean:
	rm -rf MLO *.img
	make clean -C $(REPO)

REPO= u-boot
DEVICE_TREES= ../device_trees
CONFIG= am335x_oresat_defconfig
PATCH_FILE= u-boot.patch


.PHONY: all clean

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS=
else
	FLAGS= ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

all: patch
	make -C $(DEVICE_TREES)
#	@cp $(DEVICE_TREES)/*.dtb u-boot/arch/arm/dts/
	make -C $(REPO) $(FLAGS) $(CONFIG)
	make -C $(REPO) $(FLAGS)
	@cp $(REPO)/MLO .
	@cp $(REPO)/*.img .

patch: clone
	@cp $(wildcard am335x_*_defconfig) $(REPO)/configs/
#	if ! patch -N -f -p1 --dry-run -s <$(PATCH_FILE); then \
#		patch -p1 <$(PATCH_FILE); \
#	fi
#	patch -p1 <$(PATCH_FILE); 

clone:
	@if [ ! -d $(REPO) ]; then \
		git clone https://github.com/beagleboard/$(REPO); \
	fi

clean:
	rm -rf MLO *.img
	make clean -C $(REPO)

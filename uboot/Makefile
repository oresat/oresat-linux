REPO= u-boot
DEVICE_TREES= ../device_trees
#CONFIG= am335x_evm_defconfig
CONFIG= am335x_oresat_defconfig
PATCH_FILE= u-boot.patch

.PHONY: all clean

ARCH= $(shell uname -m)
ifeq ($(ARCH), armv7l)
	FLAGS=
else
	FLAGS= ARCH=arm CROSS_COMPILE=arm-none-eabi-
endif

all: pre
	make -C $(REPO) $(FLAGS)
	@cp $(REPO)/MLO .
	@cp $(REPO)/*.img .

debug: pre
	make CONFIG_TOOLS_DEBUG=1 -C $(REPO) $(FLAGS) 
	@cp $(REPO)/MLO .
	@cp $(REPO)/*.img .

pre: clone 
	make -C $(REPO) $(FLAGS) mrproper
	make -C $(REPO) $(FLAGS) $(CONFIG)

#patch: clone
#	@cp $(wildcard am335x_*_defconfig) $(REPO)/configs/
#	if ! patch -N -f -p1 --dry-run -s <$(PATCH_FILE); then \
#		patch -p1 <$(PATCH_FILE); \
#	fi
#	patch -p1 <$(PATCH_FILE); 

clone:
	@if [ ! -d $(REPO) ]; then \
		#git clone https://github.com/beagleboard/$(REPO); \
		git clone -b board/oresat/test git@github.com:chroco/u-boot.git; \
	fi

clean:
	rm -rf MLO *.img
	make clean -C $(REPO)

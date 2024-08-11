REPO= u-boot
DEVICE_TREES= ../device_trees
DEFCONFIG= am335x_oresat_defconfig
PATCH_FILE= u-boot.patch

.PHONY: all pre clone clean

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

pre: clone 
	make -C $(REPO) $(FLAGS) mrproper
	make -C $(REPO) $(FLAGS) $(DEFCONFIG)

clone:
	@if [ -z "$(ls -A $(REPO))" ] ; then \
		git submodule update --init ; \
	fi

clean:
	rm -rf MLO *.img
	make clean -C $(REPO)

REPO= u-boot

.PHONY: all clean

all: config
	make -C $(REPO) ARCH=arm CROSS_COMPILE=arm-none-eabi-
	@cp $(REPO)/MLO .
	@cp $(REPO)/*.img .

config: clone
	@cp am335x_oresat_defconfig $(REPO)/configs/
	@if [ ! -f "$(REPO)/.config" ]; then \
		make -C $(REPO) ARCH=arm CROSS_COMPILE=arm-none-eabi- am335x_oresat_defconfig; \
	fi

clone:
	@if [ ! -d $(REPO) ]; then \
		git clone https://github.com/beagleboard/$(REPO); \
	fi

clean:
	rm -rf MLO *.img
	make clean -C $(REPO)

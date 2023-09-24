DTS = am335x-oresat-card.dts
DTB = $(DTS:%.dts=%.dtb)

REPO = BeagleBoard-DeviceTrees

.PHONY: all clean clone

all: clone $(DTB)

clone:
	@if [ ! -d $(REPO) ]; then \
			git clone https://github.com/beagleboard/$(REPO); \
	fi

%.dtb: %.dts
	@cp $< $(REPO)/src/arm/
	make -C $(REPO) clean src/arm/$@
	@cp $(REPO)/src/arm/$@ .

clean:
	rm $(DTB)
	make -C $(REPO) clean

install:
	cp $(DTB) /boot/dtbs/*/am335x-pocketbeagle.dts

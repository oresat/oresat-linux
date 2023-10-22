SOURCES = $(wildcard *.dts)
DTBS = $(SOURCES:%.dts=%.dtb)

REPO = BeagleBoard-DeviceTrees

.PHONY: all clean clone

all: clone $(DTBS)

clone:
	@if [ ! -d $(REPO) ]; then \
		git clone https://github.com/beagleboard/$(REPO); \
	fi
	@cp $(wildcard *.dtsi) $(REPO)/src/arm/

%.dtb: %.dts
	@cp $< $(REPO)/src/arm/
	make -C $(REPO) src/arm/$@
	@cp $(REPO)/src/arm/$@ .

clean:
	rm $(DTBS)
	make -C $(REPO) clean

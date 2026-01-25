SOURCES = $(wildcard *.dts)
DTBS = $(SOURCES:%.dts=%.dtb)

REPO = BeagleBoard-DeviceTrees
SRC_PATH = src/arm/ti/omap

.PHONY: all clean clone

all: clone $(DTBS)

# copy all dtsi files into the source directory
clone:
	@cp $(wildcard *.dtsi) $(REPO)/$(SRC_PATH)

%.dtb: %.dts
	@cp $< $(REPO)/$(SRC_PATH)/
	make -C $(REPO) $(SRC_PATH)/$@
	@cp $(REPO)/$(SRC_PATH)/$@ .

clean:
	rm -f *.dtb
	make -C $(REPO) clean

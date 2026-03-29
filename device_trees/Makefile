SOURCES = $(wildcard *.dts)
DTBS = $(SOURCES:%.dts=%.dtb)

REPO = BeagleBoard-DeviceTrees
SRC_PATH = src/arm/ti/omap

.PHONY: all clean copy-to-repo

all: copy-to-repo $(DTBS)

# copy all dtsi files into the source directory
copy-to-repo:
	@cp $(wildcard *.dtsi) $(REPO)/$(SRC_PATH)

%.dtb: %.dts
	@cp $< $(REPO)/$(SRC_PATH)/
	make -C $(REPO) $(SRC_PATH)/$@
	@mv $(REPO)/$(SRC_PATH)/$@ .

clean:
	rm -f *.dtb
	rm -f $(REPO)/$(SRC_PATH)/oresat-*
	make -C $(REPO) clean

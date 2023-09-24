SOURCES = \
	ORESAT-CFC-00A0.dts \
	ORESAT-C3-00A0.dts \
	ORESAT-DXWIFI-00A0.dts \
	ORESAT-GPS-00A0.dts \
  ORESAT-STAR-TRACKER-00A0.dts

DTC_FLAGS= -Wno-unit_address_vs_reg -Wno-chosen_node_is_root -Wno-alias_paths
DTBOS = $(SOURCES:%.dts=%.dtbo)

.PHONY: all clean install

%.dtbo: %.dts
	cpp -nostdinc -I include -undef -x assembler-with-cpp $< $<.tmp
	dtc -O dtb -o $@ -b 0 -@ $(DTC_FLAGS) $<.tmp

all: $(DTBOS)

clean:
	rm -rf *.dtbo *.tmp

install:
	cp *.dtbo /lib/firmware/

SOURCES = \
	cfc-00A0.dts \
	dxwifi-00A0.dts \
	gps-00A0.dts \
	c3-00A0.dts

DTC_FLAGS= -Wno-unit_address_vs_reg -Wno-chosen_node_is_root -Wno-alias_paths
DTBOS = $(SOURCES:%.dts=%.dtbo)

.PHONY: all clean

%.dtbo: %.dts
	cpp -nostdinc -I include -undef -x assembler-with-cpp $< $<.tmp
	dtc -O dtb -o $@ -b 0 -@ $(DTC_FLAGS) $<.tmp

all: $(DTBOS)

clean:
	rm -rf *.dtbo *.tmp


SOURCES = $(wildcard *.dts)
CPP_FLAGS= -nostdinc -I include -undef -x assembler-with-cpp
DTC_FLAGS= -Wno-unit_address_vs_reg -Wno-chosen_node_is_root -Wno-alias_paths
DTBOS = $(SOURCES:%.dts=%.dtbo)

.PHONY: all clean install

%.dtbo: %.dts
	# use cpp (c preprocessor) to replace #defines with actual values in .tmp files
	cpp $(CPP_FLAGS) $< $<.tmp
	# complile .tmp files into .dtbo
	dtc -O dtb -o $@ -b 0 -@ $(DTC_FLAGS) $<.tmp

all: $(DTBOS)

clean:
	rm -rf *.dtbo *.tmp

install:
	cp *.dtbo /lib/firmware/

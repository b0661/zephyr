INCLUDE += subsys

ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
ARCH := arm
GCC := $(ZEPHYR_SDK_INSTALL_DIR)/sysroots/x86_64-pokysdk-linux/usr/bin/arm-zephyr-eabi/arm-zephyr-eabi-gcc
DTC := $(ZEPHYR_SDK_INSTALL_DIR)/sysroots/x86_64-pokysdk-linux/usr/bin/dtc

include $(ZEPHYR_BASE)/tests/unit/Makefile.unittest

$(O)/test.dts_compiled: test.dts
	mkdir -p $(@D)
	echo '#include "'$(realpath $<)'"' > $(O)/test.dts_pre_compiled
	if test -e $(O)/test.overlay; then echo '#include "'$(O)/test.overlay'"' >> $(O)/test.dts_pre_compiled ; fi ;
	$(GCC) -E -Wp,-MD,$(O)/test.dts_compiled.d.pre.tmp -nostdinc \
		-I$(ROOT_DIR) \
		-I$(ZEPHYR_BASE)/include \
		-I$(ZEPHYR_BASE)/kernel/include \
		-I$(ZEPHYR_BASE)/arch/$(ARCH)/include \
		-I$(ZEPHYR_BASE)/arch/$(ARCH)/soc \
		-I$(ZEPHYR_BASE)/lib/libc/minimal/include \
		-I$(ZEPHYR_BASE)/dts \
		-I$(ZEPHYR_BASE)/dts/common \
		-I$(ZEPHYR_BASE)/dts/$(ARCH) \
		-undef -D__DTS__ -x assembler-with-cpp -o $(O)/test.dts_compiled.dts.tmp $(O)/test.dts_pre_compiled
	$(DTC) -O dts -o $(O)/test.dts_compiled -b 0 -i $(ZEPHYR_BASE)/dts/$(ARCH)/  -d $(O)/test.dts_compiled.d.dtc.tmp $(O)/test.dts_compiled.dts.tmp
	cat $(O)/test.dts_compiled.d.pre.tmp $(O)/test.dts_compiled.d.dtc.tmp > $(O)/test.dts_compiled.d

$(O)/generated_dts.h: $(O)/test.dts_compiled
	mkdir -p $(@D)
	cp -R $(ZEPHYR_BASE)/dts/$(ARCH)/yaml $(O)
	cp "test,pinmux.yaml" $(O)/yaml
	cp "test,device.yaml" $(O)/yaml
	$(ZEPHYR_BASE)/scripts/extract_dts_includes.py -d $(realpath $<) -y $(O)/yaml > $@

$(O)/main.o : main.c $(O)/generated_dts.h
	mkdir -p $(@D)
	$(CC) -I$(ZEPHYR_BASE) $(CFLAGS) $(INCLUDED) -c $(realpath $<) -o $@

SRC_DIR = src/driver/sd_card/rpi
include $(GENODE_DIR)/repos/base/recipes/src/content.inc

MIRROR_FROM_OS_DIR := src/driver/sd_card/adma2.cc \
                      src/driver/sd_card/adma2.h \
                      src/driver/sd_card/benchmark.h \
                      src/driver/sd_card/driver_base.h \
                      src/driver/sd_card/main.cc \
                      src/driver/sd_card/sd_card.h

content: $(MIRROR_FROM_OS_DIR)

$(MIRROR_FROM_OS_DIR):
	cp $(GENODE_DIR)/repos/os/$@ $@

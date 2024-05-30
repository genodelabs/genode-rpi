SRC_DIR = src/drivers/sd_card/rpi
include $(GENODE_DIR)/repos/base/recipes/src/content.inc

MIRROR_FROM_OS_DIR := src/drivers/sd_card/adma2.cc \
                      src/drivers/sd_card/adma2.h \
                      src/drivers/sd_card/benchmark.h \
                      src/drivers/sd_card/driver_base.h \
                      src/drivers/sd_card/main.cc \
                      src/drivers/sd_card/sd_card.h

content: $(MIRROR_FROM_OS_DIR)

$(MIRROR_FROM_OS_DIR):
	cp $(GENODE_DIR)/repos/os/$@ $@

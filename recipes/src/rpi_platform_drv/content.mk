SRC_DIR = src/drivers/platform/rpi
include $(GENODE_DIR)/repos/base/recipes/src/content.inc

MIRROR_FROM_OS_DIR := src/drivers/platform/clock.h \
                      src/drivers/platform/device.cc \
                      src/drivers/platform/device_component.cc \
                      src/drivers/platform/device_component.h \
                      src/drivers/platform/device.h \
                      src/drivers/platform/device_model_policy.cc \
                      src/drivers/platform/named_registry.h \
                      src/drivers/platform/policy.h \
                      src/drivers/platform/power.h \
                      src/drivers/platform/reset.h \
                      src/drivers/platform/root.cc \
                      src/drivers/platform/root.h \
                      src/drivers/platform/session_component.cc \
                      src/drivers/platform/session_component.h \
                      src/drivers/platform/types.h

content: $(MIRROR_FROM_OS_DIR)

$(MIRROR_FROM_OS_DIR):
	cp $(GENODE_DIR)/repos/os/$@ $@

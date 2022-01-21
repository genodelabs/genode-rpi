TARGET   = rpi_platform_drv
REQUIRES = arm_v6
SRC_CC   = device.cc
SRC_CC  += device_component.cc
SRC_CC  += device_model_policy.cc
SRC_CC  += main.cc
SRC_CC  += session_component.cc
SRC_CC  += root.cc
LIBS     = base

PLATFORM_OS_DIR := $(dir $(call select_from_repositories,src/drivers/platform/device.cc))
INC_DIR          = $(PRG_DIR) $(PLATFORM_OS_DIR)

vpath main.cc $(PRG_DIR)
vpath %.cc    $(PLATFORM_OS_DIR)

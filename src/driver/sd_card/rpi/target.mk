TARGET   = rpi_sd_card
REQUIRES = arm_v6

GENERIC_DRIVER_DIR := $(dir $(call select_from_repositories,src/driver/sd_card/sd_card.h))

SRC_CC  += main.cc driver.cc
LIBS    += base
INC_DIR += $(PRG_DIR) $(GENERIC_DRIVER_DIR)

vpath %.cc $(GENERIC_DRIVER_DIR)
vpath %.cc $(PRG_DIR)

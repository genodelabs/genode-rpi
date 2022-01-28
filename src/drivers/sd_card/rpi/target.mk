TARGET   = rpi_sd_card_drv
REQUIRES = arm_v6

GENERIC_DRV_DIR := $(dir $(call select_from_repositories,src/drivers/sd_card/sd_card.h))

SRC_CC  += main.cc driver.cc
LIBS    += base
INC_DIR += $(PRG_DIR) $(GENERIC_DRV_DIR)

vpath %.cc $(GENERIC_DRV_DIR)
vpath %.cc $(PRG_DIR)

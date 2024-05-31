DRIVER   := rpi_usb_host
TARGET   := $(DRIVER)
REQUIRES := arm_v6
LIBS      = base rpi_linux_generated rpi_lx_emul jitterentropy
INC_DIR   = $(PRG_DIR)
SRC_CC   += main.cc
SRC_C    += dummies.c
SRC_C    += lx_emul.c
SRC_C    += lx_emul/shadow/drivers/char/random.c
SRC_C    += lx_emul/shadow/mm/dmapool.c
SRC_CC   += lx_emul/shared_dma_buffer.cc
SRC_CC   += lx_emul/random.cc
SRC_C    += lx_emul/usb.c
SRC_C    += $(notdir $(wildcard $(PRG_DIR)/generated_dummies.c))

# Genode C-API backends
SRC_CC   += genode_c_api/usb.cc

vpath genode_c_api/usb.cc $(subst /genode_c_api,,$(call select_from_repositories,src/lib/genode_c_api))

DWC_DEF  = -DDWC_GENODE -DDWC_LINUX -DDWC_LIBMODULE -DDWC_CCLIB -DDWC_NOTIFYLIB
DWC_DEF += -DDWC_UTFLIB -DDWC_HS_ELECT_TST -DPLATFORM_INTERFACE
DWC_DEF += -I$(LX_SRC_DIR)/drivers/usb/host/dwc_common_port
DWC_DEF += -I$(LX_SRC_DIR)/drivers/usb/host/dwc_otg

CC_OPT_drivers/usb/host/dwc_common_port/dwc_cc            += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_common_port/dwc_common_linux  += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_common_port/dwc_crypto        += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_common_port/dwc_dh            += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_common_port/dwc_mem           += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_common_port/dwc_modpow        += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_common_port/dwc_notifier      += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_adp               += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_attr              += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_cil_intr          += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_cil               += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_driver            += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_fiq_fsm           += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_fiq_stub          += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_hcd_ddma          += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_hcd_intr          += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_hcd_linux         += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_hcd               += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_hcd_queue         += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_pcd_intr          += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_pcd_linux         += $(DWC_DEF)
CC_OPT_drivers/usb/host/dwc_otg/dwc_otg_pcd               += $(DWC_DEF)

CC_OPT_drivers/base/regmap/regmap          += -I$(LX_SRC_DIR)/drivers/base/regmap
CC_OPT_drivers/usb/gadget/function/f_hid   += -I$(LX_SRC_DIR)/drivers/usb/gadget
CC_OPT_drivers/usb/gadget/function/f_rndis += -I$(LX_SRC_DIR)/drivers/usb/gadget
CC_OPT_drivers/usb/gadget/legacy/cdc2      += -I$(LX_SRC_DIR)/drivers/usb/gadget/function
CC_OPT_drivers/usb/gadget/legacy/ether     += -I$(LX_SRC_DIR)/drivers/usb/gadget/function
CC_OPT_drivers/usb/gadget/legacy/hid       += -I$(LX_SRC_DIR)/drivers/usb/gadget/function
CC_OPT_drivers/usb/gadget/legacy/serial    += -I$(LX_SRC_DIR)/drivers/usb/gadget/function
CC_OPT_drivers/usb/gadget/udc/trace        += -I$(LX_SRC_DIR)/drivers/usb/gadget/udc

# Driver-specific device-tree binary data
BOARDS           := rpi
DTS_EXTRACT(rpi) := --select usb

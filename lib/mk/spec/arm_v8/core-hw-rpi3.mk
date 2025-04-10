REQUIRES = hw

REP_INC_DIR += src/core/board/rpi3

# add C++ sources
SRC_CC += spec/arm/bcm2835_pic.cc
SRC_CC += spec/arm/bcm2837_pic.cc
SRC_CC += kernel/vcpu_thread_off.cc
SRC_CC += platform_services.cc

vpath spec/arm/%.cc $(REP_DIR)/src/core

# include less specific configuration
include $(call select_from_repositories,lib/mk/spec/arm_v8/core-hw.inc)

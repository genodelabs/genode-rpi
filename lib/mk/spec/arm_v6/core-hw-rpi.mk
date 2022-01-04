#
# \brief  Build config for Genodes core process
# \author Norman Feske
# \date   2013-04-05
#

REQUIRES = hw

# add include paths
REP_INC_DIR += src/core/board/rpi
REP_INC_DIR += src/core/spec/arm_v6

# add C++ sources
SRC_CC += kernel/cpu_up.cc
SRC_CC += kernel/lock.cc
SRC_CC += kernel/vm_thread_off.cc
SRC_CC += platform_services.cc
SRC_CC += spec/arm/bcm2835_pic.cc
SRC_CC += spec/arm/system_timer.cc
SRC_CC += spec/arm_v6/perf_counter.cc

SRC_S  += spec/arm/vfpv2.s

vpath spec/arm/%.cc $(REP_DIR)/src/core

# include less specific configuration
include $(call select_from_repositories,lib/mk/spec/arm/core-hw.inc)

CC_CXX_WARN_STRICT_CONVERSION =

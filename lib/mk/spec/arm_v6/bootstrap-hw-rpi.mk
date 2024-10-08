REQUIRES = hw

REP_INC_DIR += src/bootstrap/board/rpi

SRC_CC  += bootstrap/platform_cpu_memory_area.cc
SRC_CC  += board/rpi/platform.cc
SRC_CC  += bootstrap/spec/arm/arm_v6_cpu.cc
SRC_S   += bootstrap/spec/arm/crt0.s

vpath board/rpi/platform.cc $(REP_DIR)/src/bootstrap

CC_MARCH = -march=armv6k+nofp -mfpu=vfp

ARCH_WIDTH_PATH := spec/32bit

include $(call select_from_repositories,lib/mk/bootstrap-hw.inc)

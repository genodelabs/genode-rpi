REQUIRES = hw

REP_INC_DIR += src/bootstrap/board/rpi3

SRC_CC  += board/rpi3/platform.cc
SRC_CC  += bootstrap/spec/arm_64/cortex_a53_mmu.cc

vpath board/rpi3/%.cc $(REP_DIR)/src/bootstrap

include $(call select_from_repositories,lib/mk/spec/arm_v8/bootstrap-hw.inc)

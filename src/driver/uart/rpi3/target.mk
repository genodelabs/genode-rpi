TARGET = rpi3_uart
REQUIRES = arm_v8a

include $(call select_from_repositories,src/driver/uart/target.inc)

TARGET = rpi3_uart_drv
REQUIRES = arm_v8a

include $(call select_from_repositories,src/drivers/uart/target.inc)

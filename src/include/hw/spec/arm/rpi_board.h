/*
 * \brief   Raspberry PI specific board definitions
 * \author  Stefan Kalkowski
 * \date    2019-05-16
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__INCLUDE__HW__SPEC__ARM__RPI_BOARD_H_
#define _SRC__INCLUDE__HW__SPEC__ARM__RPI_BOARD_H_

#include <drivers/uart/pl011.h>
#include <hw/spec/arm/boot_info.h>

namespace Hw::Rpi_board {

	using Serial = Genode::Pl011_uart;

	enum {
		RAM_0_BASE             = 0x00000000,
		RAM_0_SIZE             = 0x10000000,

		PL011_0_MMIO_BASE      = 0x20201000,
		PL011_0_MMIO_SIZE      = 0x1000,

		SYSTEM_TIMER_IRQ       = 1,
		SYSTEM_TIMER_MMIO_BASE = 0x20003000,
		SYSTEM_TIMER_MMIO_SIZE = 0x1000,
		SYSTEM_TIMER_CLOCK     = 1000000,

		IRQ_CONTROLLER_BASE    = 0x2000b200,
		IRQ_CONTROLLER_SIZE    = 0x100,

		UART_BASE              = 0x20201000,
		UART_CLOCK             = 3000000,

		USB_DWC_OTG_BASE       = 0x20980000,
		USB_DWC_OTG_SIZE       = 0x20000,

		DWC_IRQ                = 9,
	};
}

#endif /* _SRC__INCLUDE__HW__SPEC__ARM__RPI_BOARD_H_ */

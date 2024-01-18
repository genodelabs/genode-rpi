/*
 * \brief  Timer driver for core
 * \author Norman Feske
 * \date   2013-04-05
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Kernel OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__CORE__BCM2835__SYSTEM_TIMER_H_
#define _SRC__CORE__BCM2835__SYSTEM_TIMER_H_

/* Kernel includes */
#include <util/mmio.h>

namespace Board { class Timer; }


/**
 * Timer driver for core
 *
 * Timer channel 0 apparently doesn't work on the RPI, so we use channel 1
 */
struct Board::Timer : Genode::Mmio<0x14>
{
	struct Cnt_ctrl_status : Register<0x0, 32>
	{
		struct Irq_1 : Bitfield<1, 1> { };
	};
	struct Cnt_low       : Register<0x4,  32> { };
	struct Cnt_high      : Register<0x8,  32> { };
	struct Cnt_compare_1 : Register<0x10, 32> { };

	/* Returns 64-bit counter value out of both 32-bit registers */
	Genode::uint64_t _counter() const;

	Timer(unsigned);

	void init();
};

#endif /* _SRC__CORE__BCM2835__SYSTEM_TIMER_H_ */

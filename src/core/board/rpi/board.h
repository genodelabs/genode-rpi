/*
 * \brief  Board driver for core
 * \author Martin Stein
 * \author Stefan Kalkowski
 * \date   2012-04-23
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__SPEC__RPI__BOARD_H_
#define _CORE__SPEC__RPI__BOARD_H_

/* base-hw internal includes */
#include <hw/spec/arm/rpi_board.h>

/* base-hw Core includes */
#include <spec/arm/bcm2835_pic.h>
#include <spec/arm/system_timer.h>
#include <spec/arm_v6/cpu.h>
#include <no_vcpu_board.h>

namespace Board {
	using namespace Hw::Rpi_board;

	struct Global_interrupt_controller
	{
		static constexpr unsigned NR_OF_IRQ = Bcm2835_pic::NR_OF_IRQ;

		/* suspend/resume not supported */
		void resume() {}
	};

	struct Local_interrupt_controller : Bcm2835_pic
	{
		Local_interrupt_controller(Global_interrupt_controller&)
		: Bcm2835_pic() {}
	};
};

#endif /* _CORE__SPEC__RPI__BOARD_H_ */

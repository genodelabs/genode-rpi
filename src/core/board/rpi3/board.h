/*
 * \brief  Board driver for core
 * \author Stefan Kalkowski
 * \date   2019-05-10
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__SPEC__RPI3__BOARD_H_
#define _CORE__SPEC__RPI3__BOARD_H_

/* base-hw internal includes */
#include <hw/spec/arm_64/rpi3_board.h>

/* base-hw Core includes */
#include <spec/arm/bcm2837_pic.h>
#include <spec/arm/generic_timer.h>
#include <spec/arm_v8/cpu.h>
#include <no_vcpu_board.h>

namespace Board {

	using namespace Hw::Rpi3_board;
	class Pic : public Bcm2837_pic
	{
		using Bcm2837_pic::Bcm2837_pic;
	};

	enum { TIMER_IRQ = 1 };
};

#endif /* _CORE__SPEC__RPI3__BOARD_H_ */

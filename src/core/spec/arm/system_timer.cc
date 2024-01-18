/*
 * \brief   Timer implementation specific to BCM2835 System Timer
 * \author  Norman Feske
 * \author  Stefan Kalkowski
 * \author  Martin Stein
 * \date    2016-01-07
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* core includes */
#include <board.h>
#include <platform.h>
#include <kernel/timer.h>

using namespace Core;
using namespace Kernel;
using Device = Board::Timer;


Genode::uint64_t Board::Timer::_counter() const
{
	uint64_t const high = read<Device::Cnt_high>();
	uint64_t const low  = read<Device::Cnt_low>();

	/* if higher counter value changed in between, re-read everything */
	return (high == (time_t)read<Device::Cnt_high>())
		? (high << 32U) | low : _counter();
}


Board::Timer::Timer(unsigned)
:
	Mmio({(char *)Platform::mmio_to_virt(Board::SYSTEM_TIMER_MMIO_BASE), Board::SYSTEM_TIMER_MMIO_SIZE})
{ }


void Timer::_start_one_shot(time_t const ticks)
{
	/* ticks is guaranteed to be less than _max_value() resp. 4-byte */
	Device::Cnt_low::access_t off = (Device::Cnt_low::access_t)
		(ticks < 2 ? 2 : ticks);
	_device.write<Device::Cnt_ctrl_status::Irq_1>(1);
	_device.read<Device::Cnt_ctrl_status>();
	_device.write<Device::Cnt_compare_1>(_device.read<Device::Cnt_low>() + off);
}


enum { TICS_PER_US = Board::SYSTEM_TIMER_CLOCK / 1000 / 1000 };


time_t Timer::ticks_to_us(time_t const ticks) const {
	return ticks / TICS_PER_US; }


time_t Timer::us_to_ticks(time_t const us) const {
	return us * TICS_PER_US; }


time_t Timer::_max_value() const {
	return 0xffffffff; }


time_t Timer::_duration() const { return _device._counter() - _time; }


unsigned Timer::interrupt_id() const { return Board::SYSTEM_TIMER_IRQ; }

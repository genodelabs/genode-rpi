/*
 * \brief   Pic implementation specific to Rpi3
 * \author  Stefan Kalkowski
 * \date    2019-05-27
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* base-hw Core includes */
#include <board.h>
#include <platform.h>


Board::Bcm2837_pic::Bcm2837_pic(Global_interrupt_controller &global_irq_ctrl)
:
	Mmio({(char *)Core::Platform::mmio_to_virt(Board::LOCAL_IRQ_CONTROLLER_BASE), Board::LOCAL_IRQ_CONTROLLER_SIZE}),
	_bcm2835_pic(global_irq_ctrl, {(char *)Board::IRQ_CONTROLLER_BASE, Board::IRQ_CONTROLLER_SIZE})
{ }


bool Board::Bcm2837_pic::take_request(unsigned & irq)
{
	Board::Cpu::Id cpu = Board::Cpu::executing_id();
	Core_irq_source<0>::access_t src = 0;
	switch (cpu.value) {
	case 0: src = read<Core_irq_source<0>>(); break;
	case 1: src = read<Core_irq_source<1>>(); break;
	case 2: src = read<Core_irq_source<2>>(); break;
	case 3: src = read<Core_irq_source<3>>(); break;
	}

	if ((1 << TIMER_IRQ) & src) {
		irq = TIMER_IRQ;
		return true;
	}

	if (0xf0 & src) {
		irq = IPI;
		switch (cpu.value) {
		case 0: write<Core_mailbox_clear<0>>(1); break;
		case 1: write<Core_mailbox_clear<1>>(1); break;
		case 2: write<Core_mailbox_clear<2>>(1); break;
		case 3: write<Core_mailbox_clear<3>>(1); break;
		}
		return true;
	}

	// Gpu interrupt
	if (cpu.value == 0 && Core_irq_source<0>::Gpu::get(src)) {
		auto result = _bcm2835_pic.take_request(irq);
		return result;
	}

	return false;
}


void Board::Bcm2837_pic::_timer_irq(Hw::Arm_cpu::Id cpu, bool enable)
{
	unsigned v = enable ? 1 : 0;
	switch (cpu.value) {
		case 0:
			write<Core_timer_irq_control<0>::Cnt_p_ns_irq>(v);
			return;
		case 1:
			write<Core_timer_irq_control<1>::Cnt_p_ns_irq>(v);
			return;
		case 2:
			write<Core_timer_irq_control<2>::Cnt_p_ns_irq>(v);
			return;
		case 3:
			write<Core_timer_irq_control<3>::Cnt_p_ns_irq>(v);
			return;
		default: ;
	}
}


void Board::Bcm2837_pic::_ipi(Hw::Arm_cpu::Id cpu, bool enable)
{
	unsigned v = enable ? 1 : 0;
	switch (cpu.value) {
		case 0:
			write<Core_mailbox_irq_control<0>>(v);
			return;
		case 1:
			write<Core_mailbox_irq_control<1>>(v);
			return;
		case 2:
			write<Core_mailbox_irq_control<2>>(v);
			return;
		case 3:
			write<Core_mailbox_irq_control<3>>(v);
			return;
		default: ;
	}
}


void Board::Bcm2837_pic::unmask(unsigned const i, Hw::Arm_cpu::Id cpu)
{
	switch (i) {
		case TIMER_IRQ: _timer_irq(cpu, true); return;
		case IPI:       _ipi(cpu, true);       return;
	}
	if (cpu.value == 0) _bcm2835_pic.unmask(i, cpu);
}


void Board::Bcm2837_pic::mask(unsigned const i)
{
	Board::Cpu::Id cpu = Board::Cpu::executing_id();
	switch (i) {
		case TIMER_IRQ: _timer_irq(cpu, false); return;
		case IPI:       _ipi(cpu, false);       return;
	}
	if (cpu.value == 0) _bcm2835_pic.mask(i);
}


void Board::Bcm2837_pic::irq_mode(unsigned, unsigned, unsigned) { }


void Board::Bcm2837_pic::send_ipi(Hw::Arm_cpu::Id cpu_target)
{
	switch (cpu_target.value) {
	case 0: write<Core_mailbox_set<0>>(1); return;
	case 1: write<Core_mailbox_set<1>>(1); return;
	case 2: write<Core_mailbox_set<2>>(1); return;
	case 3: write<Core_mailbox_set<3>>(1); return;
	}
}

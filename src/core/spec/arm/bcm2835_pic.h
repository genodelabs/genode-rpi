/*
 * \brief  Programmable interrupt controller for core
 * \author Norman Feske
 * \date   2013-04-05
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__BCM2835__PIC_H_
#define _CORE__BCM2835__PIC_H_

/* Genode includes */
#include <util/mmio.h>

#include <hw/spec/arm/cpu.h>

namespace Board { class Bcm2835_pic; }


class Board::Bcm2835_pic : Genode::Mmio<0x28>
{
	public:

		enum {
			NR_OF_IRQ = 64,

			/*
			 * dummy IPI value on non-SMP platform,
			 * only used in interrupt reservation within generic code
			 */
			IPI,
		};

	private:

		struct Irq_pending_basic : Register<0x0, 32>
		{
			struct Timer : Bitfield<0, 1> { };
			struct Gpu   : Bitfield<8, 2> { };
		};

		struct Irq_pending_gpu_1  : Register<0x04, 32> { };
		struct Irq_pending_gpu_2  : Register<0x08, 32> { };
		struct Irq_enable_gpu_1   : Register<0x10, 32> { };
		struct Irq_enable_gpu_2   : Register<0x14, 32> { };
		struct Irq_enable_basic   : Register<0x18, 32> { };
		struct Irq_disable_gpu_1  : Register<0x1c, 32> { };
		struct Irq_disable_gpu_2  : Register<0x20, 32> { };
		struct Irq_disable_basic  : Register<0x24, 32> { };

		class Usb_dwc_otg : Genode::Mmio<0x40c>
		{
			private:

				struct Core_irq_status : Register<0x14, 32>
				{
					struct Sof : Bitfield<3, 1> { };
				};

				struct Guid : Register<0x3c, 32>
				{
					struct Num : Bitfield<0, 14> { };

					/*
					 * The USB driver set 'Num' to a defined value
					 */
					struct Num_valid : Bitfield<31, 1> { };

					/*
					 * Filter is not used, overridden by the USB driver
					 */
					struct Kick : Bitfield<30, 1> { };
				};

				struct Host_frame_number : Register<0x408, 32>
				{
					struct Num : Bitfield<0, 14> { };
				};

				Bcm2835_pic &_global_irq_ctrl;

				bool _is_sof() const
				{
					return read<Core_irq_status::Sof>();
				}

				static bool _need_trigger_sof(Genode::uint32_t host_frame,
				                              Genode::uint32_t scheduled_frame);

			public:

				Usb_dwc_otg(Bcm2835_pic &global_irq_ctrl);

				bool handle_sof();
		};

		Usb_dwc_otg _usb;

		int _sof_cnt { 0 };

		/**
		 * Return true if specified interrupt is pending
		 */
		static bool _is_pending(unsigned i, Genode::uint32_t p1,
		                        Genode::uint32_t p2)
		{
			return i < 32 ? (p1 & (1 << i)) : (p2 & (1 << (i - 32)));
		}

		/*
		 * Noncopyable
		 */
		Bcm2835_pic(Bcm2835_pic const &);
		Bcm2835_pic &operator = (Bcm2835_pic const &);

	public:

		Bcm2835_pic();

		bool take_request(unsigned &irq);
		void finish_request() { }
		void mask();
		void unmask(unsigned const i, Hw::Arm_cpu::Id);
		void mask(unsigned const i);
		void irq_mode(unsigned, unsigned, unsigned);

		static constexpr bool fast_interrupts() { return false; }

		/* suspend/resume is not supported on this platform */
		void resume() { }

		int increment_and_return_sof_cnt() { return ++_sof_cnt; }

		void reset_sof_cnt() { _sof_cnt = 0; }
};

#endif /* _CORE__BCM2835__PIC_H_ */

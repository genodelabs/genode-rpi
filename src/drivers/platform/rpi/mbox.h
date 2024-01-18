/*
 * \brief  Mbox for communicating between Videocore and ARM
 * \author Norman Feske
 * \date   2013-09-14
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__PLATFORM__SPEC__RPI__MBOX_H_
#define _DRIVERS__PLATFORM__SPEC__RPI__MBOX_H_

/* Genode includes */
#include <util/mmio.h>
#include <util/construct_at.h>
#include <base/env.h>
#include <os/attached_mmio.h>
#include <base/attached_ram_dataspace.h>
#include <base/log.h>
#include <dataspace/client.h>
#include <timer_session/connection.h>
#include <platform_session/device.h>

class Mbox : Genode::Attached_mmio<0xbc>
{
	private:

		Genode::Env &_env;

		enum { verbose = false };

		typedef Genode::addr_t           addr_t;
		typedef Genode::uint32_t         uint32_t;
		typedef Genode::Dataspace_client Dataspace_client;

		/* according to https://www.raspberrypi.org/forums/viewtopic.php?t=165529
		 * write and read mailboxes have its own status registers; linux driver
		 * does that too */
		struct Read : Register<0x80, 32> {
			struct Channel      : Bitfield<0, 4>  { };
			struct Value        : Bitfield<4, 26> { };
			struct Cache_policy : Bitfield<30, 2> { };
		};

		struct ReadStatus : Register<0x98, 32>
		{
			struct Empty : Bitfield<30, 1> { };
			struct Full  : Bitfield<31, 1> { };
		};

		struct Write : Register<0xa0, 32>
		{
			struct Channel      : Bitfield<0, 4>  { };
			struct Value        : Bitfield<4, 26> { };
			struct Cache_policy : Bitfield<30, 2> { };
		};

		struct WriteStatus : Register<0xb8, 32>
		{
			struct Empty : Bitfield<30, 1> { };
			struct Full  : Bitfield<31, 1> { };
		};

		enum { MSG_BUFFER_SIZE = 0x1000 };
		Genode::Attached_ram_dataspace _msg_buffer { _env.ram(), _env.rm(),
		                                             MSG_BUFFER_SIZE,
		                                             Genode::UNCACHED};

		addr_t const _msg_phys = { _env.pd().dma_addr(_msg_buffer.cap()) };

		struct Delayer : Mmio::Delayer
		{
			Timer::Connection timer;
			void usleep(Genode::uint64_t us) override { timer.usleep(us); }

			Delayer(Genode::Env &env) : timer(env) { }
		} _delayer { _env };

		template <typename MESSAGE>
		MESSAGE &_message()
		{
			return *_msg_buffer.local_addr<MESSAGE>();
		}

	public:

		using Range = Platform::Device_interface::Range;

		Mbox(Genode::Env &env, Range io_mem)
		: Attached_mmio(env, {(char *)io_mem.start, io_mem.size}), _env(env) { }

		/**
		 * Return reference to typed message buffer
		 */
		template <typename MESSAGE, typename... ARGS>
		MESSAGE &message(ARGS... args)
		{
			return *Genode::construct_at<MESSAGE>(_msg_buffer.local_addr<void>(), args...);
		}

		template <typename MESSAGE>
		void call()
		{
			_message<MESSAGE>().finalize();

			if (verbose)
				_message<MESSAGE>().dump("Input");

			/* flush pending data in the read buffer */
			while (!read<ReadStatus::Empty>())
				read<Read>();

			try { wait_for(Attempts(500), Microseconds(1), _delayer,
			               WriteStatus::Full::Equal(0)); }
			catch (Polling_timeout) {
				Genode::error("Mbox: timeout waiting for ready-to-write");
				return;
			}

			Write::access_t value = 0;
			Write::Channel::     set(value, MESSAGE::channel());
			Write::Value::       set(value, (uint32_t) _msg_phys >> Write::Value::SHIFT);
			Write::Cache_policy::set(value, MESSAGE::cache_policy());
			write<Write>(value);

			bool response_received = false;
			while (!response_received) {
				try { wait_for(Attempts(500), Microseconds(1), _delayer,
				               ReadStatus::Empty::Equal(0)); }
				catch (Polling_timeout) {
					Genode::error("Mbox: timeout waiting for response");
					return;
				}
				Read::access_t response_value = read<Read>();
				uint32_t response_channel = Read::Channel::get(response_value);
				response_received = (response_channel == MESSAGE::channel());
				if (!response_received) {
					Genode::warning("Mbox: received response for channel ", response_channel,
					                " but expected ", MESSAGE::channel(), ". Retrying");
				}
			}

			if (_message<MESSAGE>().code == MESSAGE::RESPONSE_ERROR)
				Genode::error("Mbox: response error");

			if (verbose)
				_message<MESSAGE>().dump("Output");
		}
};

#endif /* _DRIVERS__PLATFORM__SPEC__RPI__MBOX_H_ */

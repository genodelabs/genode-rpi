/*
 * \brief  Raspberry Pi USB driver Linux port
 * \author Stefan Kalkowski
 * \date   2023-06-29
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/env.h>

#include <lx_emul/init.h>
#include <lx_emul/shared_dma_buffer.h>
#include <lx_emul/usb.h>
#include <lx_kit/env.h>
#include <lx_kit/init.h>
#include <lx_user/io.h>

#include <genode_c_api/usb.h>

using namespace Genode;


struct Main
{
	Env                  & env;
	Attached_rom_dataspace dtb_rom        { env, "dtb"           };
	Signal_handler<Main>   signal_handler { env.ep(), *this,
	                                        &Main::handle_signal };
	Sliced_heap            sliced_heap    { env.ram(), env.rm()  };

	void handle_signal()
	{
		lx_user_handle_io();
		Lx_kit::env().scheduler.execute();
		genode_usb_notify_peers();
	}

	Main(Env & env) : env(env)
	{
		Lx_kit::initialize(env, signal_handler);
		env.exec_static_constructors();

		Genode_c_api::initialize_usb_service(env, signal_handler,
		                                     lx_emul_shared_dma_buffer_allocate,
		                                     lx_emul_shared_dma_buffer_free,
		                                     lx_emul_usb_release_device);
		lx_emul_start_kernel(dtb_rom.local_addr<void>());
	}
};


void Component::construct(Env & env) {
	static Main main(env); }

/*
 * \brief  Platform driver for Raspberry Pi
 * \author Stefan Kalkowski
 * \date   2020-04-12
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>
#include <power_domains.h>
#include <root.h>
#include <rpi_mbox_root.h>

namespace Driver { struct Main; };

struct Driver::Main
{
	void update_config();

	Env                  & env;
	Heap                   heap           { env.ram(), env.rm()    };
	Sliced_heap            sliced_heap    { env.ram(), env.rm()    };
	Attached_rom_dataspace config         { env, "config"          };
	Device_model           devices        { heap                   };
	Signal_handler<Main>   config_handler { env.ep(), *this,
	                                        &Main::update_config   };
	Driver::Root           root           { env, sliced_heap,
	                                        config, devices        };

	Constructible<Mbox>           mbox          { };
	Constructible<Rpi_mbox::Root> mbox_root     { };
	Constructible<Power_domains>  power_domains { };

	Main(Genode::Env & e)
	: env(e)
	{
		devices.update(config.xml());
		config.sigh(config_handler);

		/* construct firmware mailbox from device info */
		devices.for_each([&] (Device & dev) {
			if (dev.type() != "broadcom-mbox")
				return;
			dev.for_each_io_mem([&] (unsigned, const Device::Io_mem &io_mem) {
				if (!mbox.constructed()) mbox.construct(env, io_mem.range); });
		});

		if (!mbox.constructed()) {
			error("Failed to find broadcom-mbox device in configuration!");
			return;
		}

		power_domains.construct(devices.powers(), *mbox);
		mbox_root.construct(env, heap, *mbox);

		env.parent().announce(env.ep().manage(*mbox_root));
		env.parent().announce(env.ep().manage(root));
	}
};


void Driver::Main::update_config()
{
	config.update();
	devices.update(config.xml());
	root.update_policy();
}

void Component::construct(Genode::Env &env) {
	static Driver::Main main(env); }

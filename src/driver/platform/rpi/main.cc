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
#include <common.h>
#include <rpi_mbox_root.h>

namespace Driver { struct Main; };

struct Driver::Main
{
	Env                  & _env;
	Attached_rom_dataspace _config_rom     { _env, "config"        };
	Signal_handler<Main>   _config_handler { _env.ep(), *this,
	                                         &Main::_handle_config };
	Common                 _common         { _env, _config_rom     };

	Constructible<Mbox>           _mbox          { };
	Constructible<Rpi_mbox::Root> _mbox_root     { };
	Constructible<Power_domains>  _power_domains { };

	void _handle_config();

	Main(Genode::Env & env);
};


void Driver::Main::_handle_config()
{
	_config_rom.update();
	_common.handle_config(_config_rom.node());
}


Driver::Main::Main(Genode::Env & env)
:
	_env(env)
{
	_config_rom.sigh(_config_handler);
	_handle_config();

	/* construct firmware mailbox from device info */
	_common.devices().for_each([&] (Device & dev) {
		if (dev.type() != "broadcom-mbox")
			return;
		dev.for_each_io_mem([&] (unsigned, const Device::Io_mem::Range &io_mem_range,
		                         const Driver::Device::Pci_bar&, bool) {
			if (!_mbox.constructed()) _mbox.construct(env, io_mem_range); });
	});

	if (!_mbox.constructed()) {
		error("Failed to find broadcom-mbox device in configuration!");
		return;
	}

	_power_domains.construct(_common.devices().powers(), *_mbox);
	_mbox_root.construct(_env, _common.heap(), *_mbox);

	_env.parent().announce(_env.ep().manage(*_mbox_root));
	_common.announce_service();
}


void Component::construct(Genode::Env &env) {
	static Driver::Main main(env); }

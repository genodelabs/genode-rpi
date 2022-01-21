/*
 * \brief  Driver for Raspberry Pi specific mbox device
 * \author Tomasz Gajewski
 * \date   2021-11-13
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _RPI_MBOX_ROOT_H_
#define _RPI_MBOX_ROOT_H_

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/log.h>
#include <base/component.h>
#include <base/heap.h>
#include <root/component.h>

/* rpi api includes */
#include <rpi_mbox_session/rpi_mbox_session.h>

/* local includes */
#include <mbox.h>
#include <property_command.h>
#include <property_message.h>


namespace Rpi_mbox {
	class Session_component;
	class Root;
	using Driver::Property_message;
}


class Rpi_mbox::Session_component : public Genode::Rpc_object<Rpi_mbox::Session>
{
	private:

		Mbox &_mbox;

	public:

		Session_component(Mbox &mbox) : _mbox(mbox) { }


		/**********************************
		 **  Rpi_mbox session interface  **
		 **********************************/

		void setup_framebuffer(Framebuffer_info &info) override
		{
			{
				auto & msg = _mbox.message<Property_message>();
				msg.append<Property_command::Get_physical_w_h>();
				_mbox.call<Property_message>();
			}

			auto &msg = _mbox.message<Property_message>();
			msg.append<Property_command::Set_physical_w_h>(info.phys_width,
			                                               info.phys_height);
			msg.append<Property_command::Set_virtual_w_h>(info.virt_width,
			                                              info.virt_height);
			msg.append<Property_command::Set_depth>(info.depth);
			auto const &res = msg.append<Property_command::Allocate_buffer>();
			_mbox.call<Property_message>();

			/* FIXME: make a function for this */
			/* bus to phys memory translation */
			info.addr = res.address & ~0xC0000000;
			info.size = res.size;
			log("setup_framebuffer ", Genode::Hex(info.addr), ", ",
			    info.size, ", ", info.phys_width, ", ", info.phys_height);
		}
};


class Rpi_mbox::Root :
	public Genode::Root_component<Rpi_mbox::Session_component>
{
	private:

		Mbox& _mbox;

	protected:

		Session_component *_create_session(const char *) override {
			return new (md_alloc()) Session_component(_mbox); }

	public:

		Root(Env& env, Allocator& md_alloc, Mbox& mbox)
		:
			Root_component<Session_component>(env.ep(), md_alloc),
			_mbox(mbox) { }
};

#endif /* _RPI_MBOX_ROOT_H_ */

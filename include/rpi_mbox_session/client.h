/*
 * \brief  Raspberry Pi specific platform session client side
 * \author Norman Feske
 * \author Tomasz Gajewski
 * \date   2013-09-16
 */

/*
 * Copyright (C) 2013-2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__RPI_MBOX_SESSION__CLIENT_H_
#define _INCLUDE__RPI_MBOX_SESSION__CLIENT_H_

#include <base/capability.h>
#include <base/rpc_client.h>
#include <rpi_mbox_session/rpi_mbox_session.h>

namespace Rpi_mbox { struct Client; }

struct Rpi_mbox::Client : Genode::Rpc_client<Rpi_mbox::Session>
{
	explicit Client(Capability<Session> session)
		: Genode::Rpc_client<Session>(session) { }

	void setup_framebuffer(Framebuffer_info &info) override {
		call<Rpc_setup_framebuffer>(info); }
};

#endif /* _INCLUDE__RPI_MBOX_SESSION__CLIENT_H_ */

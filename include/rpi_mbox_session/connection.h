/*
 * \brief  Connection to platform service
 * \author Stefan Kalkowski
 * \date   2013-04-29
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__RPI_MBOX_SESSION__CONNECTION_H_
#define _INCLUDE__RPI_MBOX_SESSION__CONNECTION_H_

#include <rpi_mbox_session/client.h>
#include <util/arg_string.h>
#include <base/connection.h>

namespace Rpi_mbox { struct Connection; }


struct Rpi_mbox::Connection : Genode::Connection<Session>, Client
{
	Connection(Genode::Env &env)
		: Genode::Connection<Session>(env, session(env.parent(),
		                              "ram_quota=6K, cap_quota=%u",
		                              CAP_QUOTA)),
		  Client(cap()) { }
};

#endif /* _INCLUDE__RPI_MBOX_SESSION__CONNECTION_H_ */

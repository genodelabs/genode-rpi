/*
 * \brief  Raspberry Pi specific platform session
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

#ifndef _INCLUDE__RPI_MBOX_SESSION__RPI_MBOX_SESSION_H_
#define _INCLUDE__RPI_MBOX_SESSION__RPI_MBOX_SESSION_H_

#include <base/capability.h>
#include <base/rpc.h>
#include <dataspace/capability.h>

namespace Rpi_mbox {
	using namespace Genode;
	struct Session;
	struct Framebuffer_info;
}


/**
 * Structure used by the protocol between the Videocore GPU and the ARM CPU for
 * setting up the framebuffer via the mbox.
 */
struct Rpi_mbox::Framebuffer_info
{
	uint32_t phys_width;
	uint32_t phys_height;
	uint32_t virt_width;
	uint32_t virt_height;
	uint32_t pitch = 0;
	uint32_t depth;
	uint32_t x_offset = 0;
	uint32_t y_offset = 0;
	uint32_t addr = 0;
	uint32_t size = 0;

	/**
	 * Default constructor needed to make the object transferable via RPC
	 */
	Framebuffer_info()
		:
			phys_width(0), phys_height(0), virt_width(0), virt_height(),
			depth(0)
	{ }

	Framebuffer_info(uint32_t width, uint32_t height, uint32_t depth)
		:
			phys_width(width), phys_height(height),
			virt_width(width), virt_height(height),
			depth(depth)
	{ }
};


struct Rpi_mbox::Session : Genode::Session
{
	/**
	 * \noapi
	 */
	static const char *service_name() { return "Rpi_mbox"; }

	enum { CAP_QUOTA = 2 };

	/**
	 * Setup framebuffer
	 *
	 * The 'info' argument serves as both input and output parameter. As input,
	 * it describes the desired properties of the framebuffer. In return, the
	 * method delivers the values that were actually taken into effect.
	 */
	virtual void setup_framebuffer(Framebuffer_info &info) = 0;


	/*********************
	 ** RPC declaration **
	 *********************/

	GENODE_RPC(Rpc_setup_framebuffer, void, setup_framebuffer, Framebuffer_info &);

	GENODE_RPC_INTERFACE(Rpc_setup_framebuffer);
};

#endif /* _INCLUDE__RPI_MBOX_SESSION__RPI_MBOX_SESSION_H_ */

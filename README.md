# Genode support for Raspberry Pi boards

This repository complements the Genode OS framework (https://genode.org) with
the board support needed to use Genode on Raspberry Pi based devices.

To use it, you first need to obtain a clone of Genode:

! git clone https://github.com/genodelabs/genode.git genode

Now, clone the _genode-rpi.git_ repository to _genode/repos/rpi_:

! git clone https://github.com/skalk/genode-rpi.git genode/repos/rpi

For enabling the board support, adjust the build-directory configuration
_etc/build.conf_ by adding the following line to the 'REPOSITORIES'
definitions.

! REPOSITORIES += $(GENODE_DIR)/repos/rpi


License
-------

Genode-specific code is provided under Genode's regular open-source license,
which is AGPLv3 + open-source linking exception. This code is also available
under a commercial license offered by Genode Labs.

For code ported from other projects - e.g., device drivers ported from the
Linux kernel - the license of the originating upstream project applies.

Please refer to the individual file headers for detailed information.

content: include/rpi_mbox_session LICENSE

include/rpi_mbox_session:
	$(mirror_from_rep_dir)

LICENSE:
	cp $(GENODE_DIR)/LICENSE $@

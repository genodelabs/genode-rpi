content: drivers.config fb.config event_filter.config en_us.chargen special.chargen

drivers.config fb.config event_filter.config:
	cp $(REP_DIR)/recipes/raw/drivers_interactive-rpi/$@ $@

en_us.chargen special.chargen:
	cp $(GENODE_DIR)/repos/os/src/server/event_filter/$@ $@

include ../config

BINDIR=$(PREFIX)/bin

INSTALL=install -m 755

LINK=-lzeda -lm

CONFIG=zeda-config
CONFIGGEN=zeda-config-gen
CHKDEP=zeda-chkdep
TARGET=$(CONFIG)

#####################################################################
all: $(TARGET)

config-gen: $(CONFIGGEN)
	@echo " INSTALL" $(CONFIGGEN)
	@$(INSTALL) $(CONFIGGEN) $(BINDIR)
$(CONFIG):
	@echo " GENERATE" $(CONFIG)
	-@rm -f $(CONFIG)
	@$(CONFIGGEN) -I $(PREFIX)/include -L $(PREFIX)/lib -l "$(LINK)" -v $(VERSION) > $(CONFIG)
install: $(CHKDEP) $(TARGET)
	@echo " INSTALL	" $(CHKDEP)
	@$(INSTALL) $(CHKDEP) $(BINDIR)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(TARGET)

include ../config

BINDIR=$(PREFIX)/bin

INSTALL=install -m 755
UNINSTALL=rm -f

INCLUDE=-I$(PREFIX)/include
LIB=-L$(PREFIX)/lib
DEF=

LINK=-lzeda -lm

ifeq ($(CONFIG_USE_LIBXML),y)
	INCLUDE += "`xml2-config --cflags`"
	LINK += "`xml2-config --libs`"
	DEF += -D__ZEDA_USE_LIBXML
endif

CHKDEP=zeda-chkdep
CONFIGGEN=zeda-config-gen
CONFIG=zeda-config

#####################################################################
all: $(CONFIG)

$(CONFIG):
	@echo " GENERATE	" $(CONFIG)
	-@rm -f $(CONFIG)
	@./$(CONFIGGEN) -I "$(INCLUDE)" -L "$(LIB)" -D "$(DEF)" -l "$(LINK)" -v $(VERSION) > $@
install: $(CHKDEP) $(CONFIGGEN) $(CONFIG)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(CONFIG)
uninstall:
	@echo " UNINSTALL	" $(CONFIGGEN)
	@$(UNINSTALL) $(BINDIR)/$(CONFIGGEN)
	@echo " UNINSTALL	" $(CHKDEP)
	@$(UNINSTALL) $(BINDIR)/$(CHKDEP)
	@echo " UNINSTALL	" $(CONFIG)
	@$(UNINSTALL) $(BINDIR)/$(CONFIG)

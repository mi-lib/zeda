include ../config

BINDIR=$(PREFIX)/bin

INSTALL=install -m 755
UNINSTALL=rm -f

INCLUDE=-I$(PREFIX)/include
LIB=-L$(PREFIX)/lib
LINK=-lzeda -lm
DEF=

ifeq ($(CONFIG_USE_LIBXML),y)
	INCLUDE += "`xml2-config --cflags`"
	LINK += "`xml2-config --libs`"
	DEF += -D__ZEDA_USE_LIBXML
endif

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
	@$(CONFIGGEN) -I "$(INCLUDE)" -L "$(LIB)" -D "$(DEF)" -l "$(LINK)" -v $(VERSION) > $@
install: $(CHKDEP) $(TARGET)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(TARGET)
uninstall:
	@echo " UNINSTALL	" $(CONFIGGEN)
	@$(UNINSTALL) $(BINDIR)/$(CONFIGGEN)
	@echo " UNINSTALL	" $(CHKDEP)
	@$(UNINSTALL) $(BINDIR)/$(CHKDEP)
	@echo " UNINSTALL	" $(TARGET)
	@$(UNINSTALL) $(BINDIR)/$(TARGET)

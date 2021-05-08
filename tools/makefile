include ../config

BINDIR=$(PREFIX)/bin

INSTALL=install -m 755
UNINSTALL=rm -f

LINK=-l$(PROJNAME)
CONFIGTOOL=$(PROJNAME)-config

include config.tools

CONFIGGEN=zeda-config-gen
TOOLS=zeda-chkdep zeda-makefile-gen $(CONFIGGEN) $(CONFIGTOOL)

#####################################################################
all: $(CONFIGTOOL)

$(CONFIGTOOL):
	@echo " GENERATE	" $@
	-@rm -f $@
	@./$(CONFIGGEN) -I "$(INCLUDE)" -L "$(LIB)" -D "$(DEF)" -l "$(LINK)" -v $(VERSION) > $@
install: $(TOOLS)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
uninstall:
	@echo " UNINSTALL	" $(TOOLS)
	@cd $(BINDIR); $(UNINSTALL) $(TOOLS)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(CONFIGTOOL)

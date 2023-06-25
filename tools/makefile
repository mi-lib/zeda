include ../libinfo
include ../config

BINDIR=$(PREFIX)/bin

INSTALL=install -m 755
UNINSTALL=rm -f

LINK=-l$(PROJNAME)
LINKCPP=-l$(PROJNAME)_cpp
CONFIGTOOL=$(shell echo ${PROJNAME} | sed 's/_/-/')-config

include config.tools

CONFIGGEN=zeda-config-gen
TOOLS=zeda-chkdep zeda-makefile-gen zeda-deb-gen $(CONFIGGEN) $(CONFIGTOOL)

#####################################################################
all: $(CONFIGTOOL)

$(CONFIGTOOL):
	@echo " GENERATE	" $@
	-@rm -f $@
	@./$(CONFIGGEN) -I "$(INCLUDE)" -L "$(LIB)" -D "$(DEF)" -l "$(LINK)" -lcpp "$(LINKCPP)" -v $(VERSION) > $@
install: $(TOOLS)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
uninstall:
	@echo " UNINSTALL	" $(TOOLS)
	@cd $(BINDIR); $(UNINSTALL) $(TOOLS)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(CONFIGTOOL)

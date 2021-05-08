MAKEFILEGEN=`which zeda-makefile-gen`

all:
	@$(MAKEFILEGEN) app | make -f -
clean:
	@$(MAKEFILEGEN) app | make -f - clean
install:
	@$(MAKEFILEGEN) app | make -f - install
uninstall:
	@$(MAKEFILEGEN) app | make -f - uninstall

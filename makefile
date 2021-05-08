MAKEFILEGEN=./tools/zeda-makefile-gen

all:
	@$(MAKEFILEGEN) | make -f -
autotest:
	@$(MAKEFILEGEN) | make -f - autotest
doc:
	@$(MAKEFILEGEN) | make -f - doc
clean:
	@$(MAKEFILEGEN) | make -f - clean
install:
	@$(MAKEFILEGEN) | make -f - install
uninstall:
	@$(MAKEFILEGEN) | make -f - uninstall

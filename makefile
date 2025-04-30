MAKEFILEGEN=./tools/zeda-makefile-gen
MAKEDEB=./tools/zeda-deb-gen

.PHONY: doc test example

all:
	@$(MAKEFILEGEN) | make -f -
test:
	@$(MAKEFILEGEN) | make -f - test
doc:
	@$(MAKEFILEGEN) | make -f - doc
example:
	@$(MAKEFILEGEN) | make -f - example
clean:
	@$(MAKEFILEGEN) | make -f - clean
install:
	@$(MAKEFILEGEN) | make -f - install
uninstall:
	@$(MAKEFILEGEN) | make -f - uninstall
deb:
	@$(MAKEDEB)

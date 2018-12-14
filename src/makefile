include ../config

ROOTDIR=..
LIBDIR=$(ROOTDIR)/lib
INCLUDE=-I$(ROOTDIR)/include

CC=gcc
CFLAGS=-ansi -Wall -fPIC -O3 $(INCLUDE) -funroll-loops
LD=gcc
LDFLAGS=-shared
SIGNUP=echo "ZEDA ver."$(VERSION)" Copyright (C) 1998 Tomomichi Sugihara (Zhidao)" >>

OBJ=zeda_misc.o\
	zeda_string.o \
	zeda_bit.o zeda_rand.o\
	zeda_array.o zeda_index.o zeda_stream.o zeda_list.o zeda_rrtab.o\
	zeda_strlist.o\
	zeda_option.o\
	zeda_bfile.o
DLIB=libzeda.so

TARGET=$(DLIB)

all: $(TARGET)

$(DLIB): $(OBJ)
	@echo " LD	" $^
	@$(LD) $(LDFLAGS) -o $@ $^ > /dev/null; $(SIGNUP) $@
	-@mv $@ $(LIBDIR)
%.o: %.c
	@echo " CC	" $<
	@$(CC) $(CFLAGS) -c $< > /dev/null
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ \#* $(TARGET)

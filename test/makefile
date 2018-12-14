include ../config

DIR=..

INCLUDE=`zeda-config -I`
LIB=`zeda-config -L`
LINK=`zeda-config -l`

CC=gcc
CFLAGS=-ansi -Wall -O3 $(LIB) $(INCLUDE) -funroll-loops

%: %.c
	@$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean :
	@rm -f *.o *~ core *test

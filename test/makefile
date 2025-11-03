LINK=`zeda-config -l`
LINKCXX=`zeda-config -lcpp`

CC=gcc
CFLAGS=-ansi -Wall -O3 -funroll-loops `zeda-config --cflags`

CXX=g++
CXXFLAGS=-std=c++17 -Wall -O3 -funroll-loops `zeda-config --cflags`

%: %.c
	@$(CC) $(CFLAGS) -o $@ $< $(LINK)
%: %.cpp
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LINKCXX)
clean :
	@rm -f *.o *~ core *test

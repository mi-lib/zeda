CFLAGS=`zeda-config --cflags`
LINK=`zeda-config -l`
LINKCPP=`zeda-config -lcpp`

TARGET=$(shell ls *.c | xargs -i basename {} .c | tr -s "\n" " ")

CXX=g++
CXXFLAGS=-std=c++17 -Wall -O3 -funroll-loops -g `zeda-config --cflags`

CC=gcc
CFLAGS+=-ansi -Wall -O3 -funroll-loops -g

all: $(TARGET)
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LINKCPP)
clean :
	-@rm -f *.o *~ core *test $(TARGET)

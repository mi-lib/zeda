CC=gcc
CFLAGS=-ansi -Wall -O3 -funroll-loops -g `zeda-config --cflags`
LINK=`zeda-config -l`

CXX=g++
CXXFLAGS=-std=c++17 -Wall -O3 -funroll-loops -g `zeda-config --cflags`
LINKCXX=`zeda-config -lcpp`

TARGET=$(shell ls *.c | xargs -I{} basename {} .c | tr -s "\n" " ") $(shell ls *.cpp | xargs -I{} basename {} .cpp | tr -s "\n" " ")

all: $(TARGET)
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LINKCXX)
clean :
	-@rm -f *.o *~ core *test $(TARGET)

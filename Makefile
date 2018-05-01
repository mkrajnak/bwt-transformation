CC=g++
CXXFLAGS=-O2 -g -Wall -Wextra -pedantic -std=c++11
LDFLAGS=-Wl,-rpath=/usr/local/lib/gcc49/
BWTED=bwted.h bwted.cpp main.h main.cpp

all: clean bwted

debug: CXXFLAGS += -DDEBUG -g
debug: all

bwted: $(BWTED)
	$(CC) $(CXXFLAGS) $(BWTED) -o bwted

clean:
	rm -f bwted

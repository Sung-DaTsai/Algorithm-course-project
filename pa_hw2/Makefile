CXX = g++
CFLAGS  = -O3 -std=c++11
DIR = src/

all: em

em: $(DIR)main.cpp Node.o 
	$(CXX)  $(CFLAGS)   $(DIR)main.cpp    Node.o  -o  em

Node.o: $(DIR)Node.cpp
	$(CXX)  $(CFLAGS)   -c  $(DIR)Node.cpp
    
clean:
	rm  *.o em

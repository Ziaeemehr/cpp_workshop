#!/bash/bin

CXX = g++
CXXFLAGS = -std=c++11 -g -lm 
objdir=obj


prog: main.o  lib.o
	$(CXX)  -o $@  $^ $(CXXFLAGS)

main.o : main.cpp lib.h
	$(CXX) -c main.cpp  $(CXXFLAGS)

lib.o : lib.cpp lib.h
	$(CXX) -c lib.cpp $(CXXFLAGS)

clean:
	rm -rf  *.pyc   

eradicate:
	rm -rf *.o  *.pyc prog

.PHONY: clean, eradicate

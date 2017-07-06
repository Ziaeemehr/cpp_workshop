#!/bash/bin

CXX = g++
CXXFLAGS = -std=c++11 -O2 -lm -fopenmp
objdir=obj
srcs=xcorr.cpp main.cpp 
objects = functions.o main.o
dependencies = functions.h 
fullobjects=$(patsubst %,$(objdir)/%,$(objects))

prog: main.o functions.o 
	$(CXX)  -o $@  $^ $(CXXFLAGS)

main.o : main.cpp xcorr.h functions.h
	$(CXX) -c main.cpp  $(CXXFLAGS)

functions.o: xcorr.cpp xcorr.h functions.h 
	$(CXX) -c xcorr.cpp $(CXXFLAGS)

clean:
	rm -f  *.pyc a.out  
eradicate:
	rm -f *.o  *.pyc *.png  prog  *.out 

.PHONY: clean, eradicate

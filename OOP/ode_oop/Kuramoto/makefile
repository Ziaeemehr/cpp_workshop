#!/bash/bin

CXX = g++
CXXFLAGS = -std=c++11  -O2 -lm -fopenmp
objdir=obj
srcs=functions.cpp main.cpp 
objects = functions.o main.o
dependencies = functions.h  

prog: main.o functions.o 
	$(CXX)  -o $@  $^ $(CXXFLAGS)

main.o : main.cpp functions.h
	$(CXX) -c main.cpp  $(CXXFLAGS)

functions.o: functions.cpp functions.h 
	$(CXX) -c functions.cpp $(CXXFLAGS)

clean:
	rm -f  *.pyc a.out *.txt *.png 
	rm -f *.png *.out 
eradicate:
	rm -f *.o  *.pyc a.out *.txt *.png 
	rm -f *.png  prog  *.out hmn

.PHONY: clean, eradicate


CXX = g++
CXXFLAGS = -std=c++11 -O2
objdir=obj

prog: main.o 
	$(CXX) -o $@ $^ $(CXXFLAGS)
main.o: main.cpp lib.h 
	$(CXX) -c main.cpp $(CXXFLAGS)

clean:
	rm  prog 
eradicate :
	rm *.o prog 

.PHONY: clean

#!/bash/bin

CXX = g++
CXXFLAGS =  -std=c++11 -O2 -lm -fopenmp
objdir=obj
srcs=functions.cpp main.cpp 
objects = functions.o main.o
dependencies = functions.h config.h 

prog: main.o functions.o 
	$(CXX)  -o $@  $^ $(CXXFLAGS)

main.o : main.cpp functions.h
	$(CXX) -c main.cpp  $(CXXFLAGS)

functions.o: functions.cpp functions.h 
	$(CXX) -c functions.cpp $(CXXFLAGS)

clean:
	rm -f  *.pyc a.out  *.png *.out  prog *.o 

.PHONY: clean


# prog: $(fullobjects)
# 	$(CXX)  -o  $@  $^  $(CXXFLAGS) 

# $(objdir)%/.o : %.cpp $(dependencies)
# 	$(CXX) -c -o $@  $<  $(CXXFLAGS) 
# ====================================

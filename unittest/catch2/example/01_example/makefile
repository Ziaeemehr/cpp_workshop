

CXX = g++
CXXFLAGS = -std=c++11 -Wall -I../

all: run
	./*.exe

01_TestCase.exe: 01_TestCase.o
	$(CXX)  -o $@  $^ $(CXXFLAGS)


%.o : %.cpp
	$(CXX) -c $<  $(CXXFLAGS)

clean:
	rm -rf *.o *.exe

.phoney: clean





# 01_TestCase.o : 01_TestCase.cpp
# 	$(CXX) -c $<  $(CXXFLAGS)

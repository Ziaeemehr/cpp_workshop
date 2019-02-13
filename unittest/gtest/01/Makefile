CXX = g++
CXXFLAGS = -g
INCS = -I./
INCSTEST =  /usr/lib
OBJS = Addition.o Multiply.o 
OBJSTEST = Addition_Test.o Multiply_Test.o

exampleapp: ExampleApp.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o exampleapp ExampleApp.cpp $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCSTEST)

testAll: $(OBJS) $(OBJSTEST)
	$(CXX) $(CXXFLAGS) $(INCSTEST) Main_TestAll.cpp $(OBJS) $(OBJSTEST)


clean:
	rm *.o exampleapp testAll 

.phoney: clean testAll exampleapp
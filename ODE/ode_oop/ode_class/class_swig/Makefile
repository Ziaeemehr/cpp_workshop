CXX=g++
CXXFLAGS  = -std=c++11 -O2 -fPIC -c 
SWIGFLAGS = -c++ -python -shadow  

# location of the Python header files
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

TARGET = example
OBJS= $(TARGET).o $(TARGET)_wrap.o

_$(TARGET).so: $(OBJS)
	$(CXX) -shared  $(OBJS) -o _$(TARGET).so
 
$(TARGET)_wrap.o: $(TARGET)_wrap.cpp
	$(CXX) $(CXXFLAGS) $(TARGET)_wrap.cpp -I $(PYTHON_INCLUDE) 

$(TARGET).o: $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $(TARGET).cpp

$(TARGET)_wrap.cpp : $(TARGET).i 
	swig $(SWIGFLAGS) -o $(TARGET)_wrap.cpp $(TARGET).i 

clean :
	rm *.o  *.so $(TARGET)_wrap.cpp $(TARGET).py *.pyc 

.PHONEY : clean 




# swig -c++ -python  -shadow number.i
# g++ -O2 -fPIC -c number.cxx
# g++ -O2 -fPIC -c number_wrap.cxx -I /usr/include/python2.7 
# g++ -shared number.o number_wrap.o -o _number.so

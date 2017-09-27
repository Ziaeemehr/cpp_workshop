swig -c++ -python functions.i
g++  -O2 -fPIC -c functions.cpp 
g++  -O2 -fPIC -c functions_wrap.cxx -I /usr/include/python2.7 
g++  -shared functions_wrap.o functions.o -o _functions.so 

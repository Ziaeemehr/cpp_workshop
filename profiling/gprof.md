g++ p5.cpp -std=c++11

### debugging, debug 
g++  -g filename.cpp
gdb a.out
(gdb) run


### gprof (profiling) 

add "-pg" to compile command
run with ./prog
gprof prog | less


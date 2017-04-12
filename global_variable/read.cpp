// How to declare a global variable that could 
// be used in the entire program
// http://stackoverflow.com/questions/2029272/how-to-declare-a-global-variable-that-could-be-used-in-the-entire-program
global.h
extern int myVar;

global.cpp
#include "global.h"
int myVar = 0;  // initialize

class1.cpp
#include "global.h"
...

class2.cpp
#include "global.h"
...

class3.cpp
#include "global.h"
...

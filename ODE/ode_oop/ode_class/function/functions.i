%module functions

%{
#define SWIG_FILE_WITH_INIT
#include "functions.h"
%}

%include "std_vector.i"

namespace std {
    %template(IntVector)     vector<int>;
    %template(DoubleVector)  vector<double>;
    %template(DoubleVector2) vector<vector<double> >;
}

%include "functions.h"
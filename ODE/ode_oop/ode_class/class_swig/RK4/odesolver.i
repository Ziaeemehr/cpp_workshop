/* File : odesolver.i */
%module odesolver

%{
#include "odesolver.h"
%}

%include stl.i
/* instantiate the required template specializations */
namespace std {
    %template(IntVector)     vector<int>;
    %template(DoubleVector)  vector<double>;
    %template(DoubleVector2) vector<vector<double> >;
}
/* Let's just grab the original header file here */
%include "odesolver.h"

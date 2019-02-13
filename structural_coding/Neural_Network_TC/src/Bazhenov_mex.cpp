/*
*	Copyright (c) 2016 Michael Schellenberger Costa mschellenbergercosta@gmail.com
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in
*	all copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*	THE SOFTWARE.
*/

/****************************************************************************************************/
/* 		Implementation of the Bazhenov2002 model as MATLAB routine (mex compiler)					*/
/* 		mex command is given by:																	*/
/* 		mex CXXFLAGS="\$CXXFLAGS -std=c++11 -O3 -fopenmp"											*/
/*			Bazhenov.cpp Pyramidal_Neuron.cpp Inhibitory_Neuron.cpp									*/
/*		The Simulation requires the following boost libraries:	Random								*/
/****************************************************************************************************/
#include "mex.h"
#include "matrix.h"

#include <vector>

#include "Data_Storage.h"
#include "Initialize_Neurons.h"
#include "Iterate_ODE.h"

/****************************************************************************************************/
/*										Fixed simulation settings									*/
/****************************************************************************************************/
typedef std::chrono::high_resolution_clock::time_point timer;
extern const int T		= 1;					/* Simulation length in s			*/
extern const int res 	= 5E4;					/* Number of iteration steps per s	*/
extern const double dt 	= 1E3/res;				/* Duration of a timestep in ms		*/
extern const std::vector<int> NumCells = {128,  /* Number of pyramidal cells		*/
                                          32,	/* Number of inhibitory cells		*/
                                          128,	/* Number of thalamocortical cells	*/
                                          32};	/* Number of reticular cells		*/
extern const int N_Cores= 7;					/* Number of CPU cores				*/
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/
mxArray* SetMexArray(int N, int M);

/****************************************************************************************************/
/*										Simulation routine	 										*/
/*										lhs defines outputs											*/
/*										rhs defines inputs											*/
/****************************************************************************************************/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    /* Seed the random number generator */
    srand(time(NULL));

    /* Initialize the populations */
    std::vector<Pyramidal_Neuron> PY;
    std::vector<Inhibitory_Neuron> IN;
    std::vector<Thalamocortical_Neuron> TC;
    std::vector<Reticular_Neuron> RE;
    setupNetwork(PY, IN, TC, RE);

    /* Data container in MATLAB format */
    std::vector<mxArray*> Data;
    Data.push_back(SetMexArray(NumCells[PYRAMIDAL],  T*res/red));	// Ve
    Data.push_back(SetMexArray(NumCells[INHIBITORY], T*res/red));	// Vi
    Data.push_back(SetMexArray(NumCells[PYRAMIDAL],  T*res/red));	// Ca

    /* Pointer to the data blocks */
    std::vector<double*> pData;
    pDate.reserve(Data.size());
    for(const auto& arrayptr : Data) {
        pData.push_back(mxGetPr(arrayptr));
    }

    /* Simulation */
    int count = 0;
    for (int t = 0; t < T*res; ++t) {
        Iterate_ODE(PY, IN, TC, RE);
        if(t%red==0){
            get_data(count++, PY, IN, TC, RE, pData);
        }

    }

    /* Return the data containers */
    nlhs = Data.size();
    for(const auto& arrayptr : Data) {
        plhs[&arrayptr - Data.data()] = arrayptr;
    }

    return;
}
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/


/****************************************************************************************************/
/*									Create MATLAB data container									*/
/****************************************************************************************************/
mxArray* SetMexArray(int N, int M) {
    mxArray* Array	= mxCreateDoubleMatrix(0, 0, mxREAL);
    mxSetM(Array, N);
    mxSetN(Array, M);
    mxSetData(Array, mxMalloc(sizeof(double)*M*N));
    return Array;
}
/****************************************************************************************************/
/*										 		end													*/
/****************************************************************************************************/

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
/*									Functions for data storage										*/
/****************************************************************************************************/
#pragma once
#include <vector>

#include "Inhibitory_Neuron.h"
#include "Pyramidal_Neuron.h"
#include "Reticular_Neuron.h"
#include "Thalamocortical_Neuron.h"

/****************************************************************************************************/
/*											Save data												*/
/****************************************************************************************************/
inline void get_data(int counter,
                     std::vector<Pyramidal_Neuron>& PY,
                     std::vector<Inhibitory_Neuron>& IN,
                     std::vector<Thalamocortical_Neuron>& TC,
                     std::vector<Reticular_Neuron>& RE,
                     std::vector<double*> pData) {
    /* Parameters for the parallelization */
    extern const int N_Cores;

    /* NOTE As C++ and Matlab have a different storage order (Row-major vs Column-major), the index
     * has to be adapted! For an NxM matrix A, element A(i,j) is accessed by A(j+i*M) rather than
     * the usual A(i+j*N)
     */
    #pragma omp parallel for num_threads(N_Cores) schedule(dynamic)
    for(unsigned i=0; i < PY.size(); i++)
        pData[0][i+PY.size()*counter] = PY[i].Vs[0];

    #pragma omp parallel for num_threads(N_Cores) schedule(dynamic)
    for(unsigned i=0; i < IN.size(); i++)
        pData[1][i+IN.size()*counter] = IN[i].V [0];

    #pragma omp parallel for num_threads(N_Cores) schedule(dynamic)
    for(unsigned i=0; i < PY.size(); i++)
        pData[2][i+PY.size()*counter] = PY[i].Ca[0];
}
/****************************************************************************************************/
/*										 		end													*/
/****************************************************************************************************/

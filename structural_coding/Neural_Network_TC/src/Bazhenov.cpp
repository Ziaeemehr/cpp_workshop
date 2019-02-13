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
/*		Main file for compilation tests																*/
/****************************************************************************************************/
#include <iostream>
#include <chrono>
#include <vector>

#include "Data_Storage.h"
#include "Initialize_Neurons.h"
#include "Iterate_ODE.h"

/****************************************************************************************************/
/*										Fixed simulation settings									*/
/****************************************************************************************************/
typedef std::chrono::high_resolution_clock::time_point timer;
extern const int T		= 1;								/* Simulation length in s				*/
extern const int res 	= 5E4;								/* Number of iteration steps per s		*/
extern const double dt 	= 1E3/res;							/* Duration of a timestep in ms			*/
extern const std::vector<int> NumCells = {128,				/* Number of pyramidal cells			*/
                                         32,				/* Number of inhibitory cells			*/
                                         128,				/* Number of thalamocortical cells		*/
                                         32};				/* Number of reticular cells			*/
extern const int N_Cores= 7;								/* Number of CPU cores					*/
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/


/****************************************************************************************************/
/*										Main simulation routine										*/
/****************************************************************************************************/
int main(void) {
    /* Seed the random number generator */
    srand(time(NULL));

    /* Initialize the populations */
    /* Take the time of the simulation */
    timer start,end;

    /* Initialize the neurons */
    std::vector<Pyramidal_Neuron> PY;
    std::vector<Inhibitory_Neuron> IN;
    std::vector<Thalamocortical_Neuron> TC;
    std::vector<Reticular_Neuron> RE;
    setupNetwork(PY, IN, TC, RE);

    /* Simulation */
    start = std::chrono::high_resolution_clock::now();
    for (int t = 0; t < T*res; ++t) {
        Iterate_ODE(PY, IN, TC, RE);
    }
    end = std::chrono::high_resolution_clock::now();

    /* Time consumed by the simulation */
    double dif = 1E-3*std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
    std::cout << "simulation done!\n";
    std::cout << "took " << dif 	<< " seconds" << "\n";
    std::cout << "end\n";
}
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/

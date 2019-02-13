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
#ifndef INITIALIZE_Neurons_H
#define INITIALIZE_Neurons_H
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif
#include <cmath>
#include <exception>
#include <utility>
#include <vector>

#include "Random_Stream.h"
#include "Inhibitory_Neuron.h"
#include "Pyramidal_Neuron.h"
#include "Reticular_Neuron.h"
#include "Thalamocortical_Neuron.h"

enum neuronType {
    PYRAMIDAL = 0,
    INHIBITORY,
    THALAMOCORTICAL,
    RETICULAR
};

static std::vector<double> getParameters(neuronType Type) {
    /* Pair containing mean and standard deviation of a gaussian distribution.*/
    std::vector<std::pair<double, double>> parameterDistribution;

    /* Get the distributions of the initial parameters */
    switch(Type) {
    case PYRAMIDAL:
        parameterDistribution = {std::make_pair(-60.95, 0.3),        /* E_L  */
                                 std::make_pair(66.7E-3, 6.7E-3),    /* g_L  */
                                 std::make_pair(1.75E-3, 0.1E-3)};   /* g_sd */
        break;
    case INHIBITORY:
        parameterDistribution = {std::make_pair(-63.8, 0.15),        /* E_L */
                                 std::make_pair(102.5E-3, 2.5E-3)};  /* g_L */
        break;
    case THALAMOCORTICAL:
        parameterDistribution = {std::make_pair(-60.95, 0.3),        /* E_L */
                                 std::make_pair(66.7E-3, 6.7E-3)};   /* g_L */
        break;
    case RETICULAR:
        parameterDistribution = {std::make_pair(-63.8, 0.15),        /* E_L */
                                 std::make_pair(102.5E-3, 2.5E-3)};  /* g_L */
        break;
    default:
        throw std::runtime_error("Unknown neuron type!");
    }

    /* Initialize the random number generators */
    std::vector<random_stream_normal> MTRand;
    MTRand.reserve(parameterDistribution.size());
    for (auto &dist : parameterDistribution) {
        MTRand.push_back(random_stream_normal(dist.first, dist.second));
    }

    /* Get the randomly distributed parameters */
    std::vector<double> parameter;
    parameter.reserve(parameterDistribution.size());
    for(auto &RNG : MTRand) {
        parameter.push_back(RNG());
    }
    return parameter;
}

template<class NEURON>
static std::vector<NEURON> initializeNeurons(neuronType type) {
    extern const std::vector<int> NumCells;

    /* Initialize the neurons */
    std::vector<NEURON> neurons;
    neurons.reserve(NumCells[type]);
    for (int i = 0; i < NumCells[type]; ++i) {
        neurons.push_back(NEURON(getParameters(type)));
    }
    return neurons;
}

static std::vector<std::vector<int>> getConnectivity(neuronType post, neuronType pre) {
    using connectome = std::vector<std::vector<int>>;
    extern const std::vector<int> NumCells;

    double length = 5*NumCells[PYRAMIDAL];
    /* Sigma for the normal distribution */
    double sigma;
    switch (pre) {
    case PYRAMIDAL:
        sigma = 250/length*NumCells[post];
        break;
    case INHIBITORY:
        sigma = 125/length*NumCells[post];
        break;
    case THALAMOCORTICAL:
        sigma = 125/length*NumCells[post];
        break;
    case RETICULAR:
        sigma = 125/length*NumCells[post];
        break;
    default:
        throw std::runtime_error("Unknown connection type!");
    }

    /* Generate the random number generator for the number of connections */
    random_stream_normal MTRand_N  = random_stream_normal(20, 5);

    /* Generate the random number generator for the target neurons */
    random_stream_normal MTRand_T = random_stream_normal(0, sigma);

    connectome connectivity(NumCells[post], std::vector<int>(0));
    for (int i=0; i < NumCells[pre]; ++i) {
        unsigned N_con = (abs((int)MTRand_N()));
        for(unsigned j=0; j < N_con; ++j) {
            int Target;
            /* Self connections are not allowed */
            do {
                Target = ((int)MTRand_T()+i)%NumCells[post];
                if(Target < 0) {
                    Target += NumCells[post];
                }
            } while (Target == i && pre == post);
            connectivity[Target].push_back(i);
        }
    }
    return connectivity;
}

void connectNeurons(std::vector<Pyramidal_Neuron>& PY,
                    std::vector<Inhibitory_Neuron>& IN,
                    std::vector<Thalamocortical_Neuron>& TC,
                    std::vector<Reticular_Neuron>& RE) {
    using connectome = std::vector<std::vector<int>>;
    /* Generate random connectivity matrices. For every Neuron[i] they store
     * the index of all neurons it RECEIVES input from
     */
    connectome conPP = getConnectivity(PYRAMIDAL, PYRAMIDAL);
    connectome conPI = getConnectivity(PYRAMIDAL, INHIBITORY);
    connectome conPT = getConnectivity(PYRAMIDAL, THALAMOCORTICAL);
    for (Pyramidal_Neuron &neuron : PY) {
        for (int connection : conPP.at(&neuron - PY.data())) {
            neuron.PY_Con.push_back(&PY.at(connection));
        }
        for (int connection : conPI.at(&neuron - PY.data())) {
            neuron.IN_Con.push_back(&IN.at(connection));;
        }
        for (int connection : conPT.at(&neuron - PY.data())) {
            neuron.TC_Con.push_back(&TC.at(connection));
        }
    }

    connectome conIP = getConnectivity(INHIBITORY, PYRAMIDAL);
    connectome conII = getConnectivity(INHIBITORY, INHIBITORY);
    connectome conIT = getConnectivity(INHIBITORY, THALAMOCORTICAL);
    for (Inhibitory_Neuron &neuron : IN) {
        for (int connection : conIP.at(&neuron - IN.data())) {
            neuron.PY_Con.push_back(&PY.at(connection));
        }
        for (int connection : conII.at(&neuron - IN.data())) {
            neuron.IN_Con.push_back(&IN.at(connection));;
        }
        for (int connection : conIT.at(&neuron - IN.data())) {
            neuron.TC_Con.push_back(&TC.at(connection));
        }
    }

    connectome conTR = getConnectivity(THALAMOCORTICAL, RETICULAR);
    connectome conTP = getConnectivity(THALAMOCORTICAL, PYRAMIDAL);
    for (Thalamocortical_Neuron &neuron : TC) {
        for (int connection : conTP.at(&neuron - TC.data())) {
            neuron.PY_Con.push_back(&PY.at(connection));
        }
        for (int connection : conTR.at(&neuron - TC.data())) {
            neuron.RE_Con.push_back(&RE.at(connection));;
        }
    }

    connectome conRT = getConnectivity(RETICULAR, THALAMOCORTICAL);
    connectome conRR = getConnectivity(RETICULAR, RETICULAR);
    connectome conRP = getConnectivity(RETICULAR, PYRAMIDAL);
    for (Reticular_Neuron &neuron : RE) {
        for (int connection : conRP.at(&neuron - RE.data())) {
            neuron.PY_Con.push_back(&PY.at(connection));
        }
        for (int connection : conRR.at(&neuron - RE.data())) {
            neuron.RE_Con.push_back(&RE.at(connection));;
        }
        for (int connection : conRT.at(&neuron - RE.data())) {
            neuron.TC_Con.push_back(&TC.at(connection));
        }
    }
}


void setupNetwork(std::vector<Pyramidal_Neuron>& PY,
                  std::vector<Inhibitory_Neuron>& IN,
                  std::vector<Thalamocortical_Neuron>& TC,
                  std::vector<Reticular_Neuron>& RE) {
    /* Initialize the individual neurons */
    PY = initializeNeurons<Pyramidal_Neuron>(PYRAMIDAL);
    IN = initializeNeurons<Inhibitory_Neuron>(INHIBITORY);
    TC = initializeNeurons<Thalamocortical_Neuron>(THALAMOCORTICAL);
    RE = initializeNeurons<Reticular_Neuron>(RETICULAR);

    connectNeurons(PY, IN, TC, RE);
}

#endif // INITIALIZE_Neurons_H

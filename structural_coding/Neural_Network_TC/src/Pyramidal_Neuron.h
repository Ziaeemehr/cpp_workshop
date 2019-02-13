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
#ifndef PYRAMIDAL_NEURON_H
#define PYRAMIDAL_NEURON_H
#pragma once
#include <cmath>
#include <vector>

#include "Inhibitory_Neuron.h"
#include "Thalamocortical_Neuron.h"

class Inhibitory_Neuron;
class Reticular_Neuron;
class Thalamocortical_Neuron;

/******************************************************************************/
/*			Implementation of the pyramidal neuron after Bazhenov2002 		  */
/******************************************************************************/
class Pyramidal_Neuron {
public:
    explicit Pyramidal_Neuron(const std::vector<double> &Param)
    : E_L(Param[0]), g_L(Param[1]), g_sd(Param[2]) {}

    /* Set strength of input current */
    void	setInput(double I) {Input = I;}

    /* ODE functions */
    void	set_RK (int);
    void 	add_RK (void);

private:
    /* Current functions */
    double	I_L		(int) const;
    double	I_Na	(int) const;
    double	I_K		(int) const;
    double	I_A		(int) const;
    double	I_KS	(int) const;
    double	I_KNa	(int) const;
    double	I_sd	(int) const;

    double	I_Ca	(int) const;
    double	I_KCa	(int) const;
    double	I_NaP	(int) const;
    double	I_AR	(int) const;

    double	I_AMPA	(int) const;
    double	I_NMDA	(int) const;
    double	I_GABA	(int) const;

    /* Gating functions */
    double alpha_h_Na(int) const;
    double alpha_n_K (int) const;
    double beta_h_Na (int) const;
    double beta_n_K  (int) const;

    double h_A_inf	(int) const;
    double m_KS_inf	(int) const;
    double tau_m_KS	(int) const;

    /* Sodium pump */
    double Na_pump	(int) const;

    /* Helper functions */
    static void add_RK(std::vector<double>& var) {
        var[0] = (-3*var[0] + 2*var[1] + 4*var[2] + 2*var[3] + var[4])/6;
    }
    static inline std::vector<double> init (double var) {
        return {var, 0.0, 0.0, 0.0, 0.0};
    }

    /* Connections (Neurons that target THIS Neuron*/
    std::vector<Pyramidal_Neuron*>  PY_Con;
    std::vector<Inhibitory_Neuron*> IN_Con;
    std::vector<Thalamocortical_Neuron*> TC_Con;

    /* Parameter constants */
    /* Membrane conductivity */
    const int		C_m		= 1;

    /* Averaged membrane area */
    const double	A_s		= 15E-5;
    const double	A_d		= 35E-5;

    /* Time constants */
    const int		tau_A	= 15;

    /* Reversal potentials */
    const double	E_L		= -60.95;
    const int		E_K		= -100;
    const int		E_Na	= 55;
    const int		E_Ca	= 120;

    const int		E_AMPA	= 0.;
    const int		E_NMDA	= 0.;
    const int		E_GABA  = -70;

    /* Channel conductivities */
    const double	g_L		= 66.7E-3;
    const double	g_Na	= 50.;
    const double	g_K		= 10.5;
    const double	g_A		= 1.;
    const double	g_KS	= 0.0686;
    const double	g_KNa	= 1.33;
    const double	g_sd	= 1.75E-3;

    const double	g_Ca	= 0.43;
    const double	g_KCa	= 0.57;
    const double	g_NaP	= 68.6E-3;
    const double	g_AR	= 25.7E-3;

    const double	g_AMPA	= 5.4E-6;
    const double	g_NMDA	= 0.9E-6;
    const double	g_GABA	= 4.15E-6;

    /* Synapse time constants */
    const int		tau_AMPA= 2;
    const int		tau_NMDA= 100;
    const int		tau_x	= 2;

    /* Calcium related constants */
    const int		alpha_Ca= 5;
    const int		tau_Ca	= 150;
    const double	Ca_0	= 2.4E-4;
    const double	K_D		= 30.;

    /* Sodium related constants */
    const int		alpha_Na= 10;
    const double	Na_0	= 9.5;
    const double	R_pump	= 0.018;

    /* Input current */
    double			Input	= 0.0;

    /* RK iteration parameters */
    const std::vector<double> A = {0.5, 0.5, 1.0, 1.0};
    const std::vector<double> B = {0.75, 0.75, 0.0, 0.0};

    /* Variables of the neuron */
    std::vector<double> 	Vd		= init(E_L),		/* Dendritic membrane voltage			*/
                    Vs		= init(E_L),		/* Somatic membrane voltage				*/
                    Ca		= init(Ca_0),		/* Calcium concentration in dendrite	*/
                    Na		= init(Na_0),		/* Sodium  concentration in soma		*/
                    h_Na	= init(0.0),		/* inactivation of Na channel			*/
                    h_A		= init(0.0),		/* inactivation of A  channel			*/
                    m_KS	= init(0.0),		/* activation 	of KS channel			*/
                    n_K		= init(0.0),   	/* activation 	of K  channel			*/
                    s_AMPA	= init(0.0),   	/* Fraction of open AMPA channels		*/
                    s_NMDA	= init(0.0),   	/* Fraction of open NMDA channels		*/
                    x_NMDA	= init(0.0);   	/* Two stage activation of NMDA channels*/

    /* Other neuron types that recieve input from this neuron type */
    friend class Inhibitory_Neuron;
    friend class Thalamocortical_Neuron;
    friend class Reticular_Neuron;

    friend void get_data(int counter,
                         std::vector<Pyramidal_Neuron>& PY,
                         std::vector<Inhibitory_Neuron>& IN,
                         std::vector<Thalamocortical_Neuron>& TC,
                         std::vector<Reticular_Neuron>& RE,
                         std::vector<double*> pData);

    friend void connectNeurons(std::vector<Pyramidal_Neuron>& PY,
                               std::vector<Inhibitory_Neuron>& IN,
                               std::vector<Thalamocortical_Neuron>& TC,
                               std::vector<Reticular_Neuron>& RE);
};

#endif // PYRAMIDAL_NEURON_H

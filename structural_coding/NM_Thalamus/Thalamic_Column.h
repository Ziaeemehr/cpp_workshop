/*
 *	Copyright (c) 2015 University of Lübeck
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
 *
 *	AUTHORS:	Michael Schellenberger Costa: mschellenbergercosta@gmail.com
 *
 *	Based on:	A thalamocortical neural mass model of the EEG during NREM sleep and its response
 *				to auditory stimulation.
 *				M Schellenberger Costa, A Weigenand, H-VV Ngo, L Marshall, J Born, T Martinetz,
 *				JC Claussen.
 *				PLoS Computational Biology http://dx.doi.org/10.1371/journal.pcbi.1005022
 */

/******************************************************************************/
/*						Implementation of a thalamic module					  */
/******************************************************************************/
#pragma once
#include <array>
#include <cmath>
#include <vector>

#include "Random_Stream.h"

class Thalamic_Column {
public:
    Thalamic_Column(double* Par)
    : g_LK (Par[0])
    , g_h (Par[1])
    {
        set_RNG();
    }

    /* Iterate one time step through SRK4 */
    void	iterate_ODE (void);
private:
    /* Declaration of private functions */
    /* Initialize the RNGs */
    void 	set_RNG		(void);

    /* Firing rates */
    double 	get_Qt		(int) const;
    double 	get_Qr		(int) const;

    /* Synaptic currents */
    double 	I_et		(int) const;
    double 	I_gt		(int) const;
    double 	I_er		(int) const;
    double 	I_gr		(int) const;

    /* Activation functions */
    double  m_inf_T_t	(int) const;
    double  m_inf_T_r	(int) const;
    double  m_inf_h		(int) const;
    double  tau_m_h		(int) const;
    double  P_h			(int) const;
    double  act_h		(void)const;

    /* Deactivation functions */
    double  h_inf_T_t	(int) const;
    double  h_inf_T_r	(int) const;
    double  tau_h_T_t	(int) const;
    double  tau_h_T_r	(int) const;

    /* Currents */
    double 	I_L_t		(int) const;
    double 	I_L_r		(int) const;
    double 	I_LK_t		(int) const;
    double 	I_LK_r		(int) const;
    double 	I_T_t		(int) const;
    double 	I_T_r		(int) const;
    double 	I_h			(int) const;

    /* Noise functions */
    double 	noise_xRK 	(int,int) const;
    double 	noise_aRK 	(int) const;

    /* ODE functions */
    void 	set_RK		(int);
    void 	add_RK	 	(void);

    /* Helper functions */
    inline std::vector<double> init (double value) {
        return {value, 0.0, 0.0, 0.0, 0.0};
    }

    inline void add_RK (std::vector<double>& var) {
        var[0] = (-3*var[0] + 2*var[1] + 4*var[2] + 2*var[3] + var[4])/6;
    }

    inline void add_RK_noise (std::vector<double>& var, unsigned noise) {
        var[0] = (-3*var[0] + 2*var[1] + 4*var[2] + 2*var[3] + var[4])/6 + noise_aRK(noise);
    }

    /* Declaration and Initialization of parameters */
    /* Membrane time in ms */
    static constexpr double 	tau_t 		= 20.;
    static constexpr double 	tau_r 		= 20.;

    /* Maximum firing rate in ms^-1 */
    static constexpr double 	Qt_max		= 400.E-3;
    static constexpr double 	Qr_max		= 400.E-3;

    /* Sigmoid threshold in mV */
    static constexpr double 	theta_t		= -58.5;
    static constexpr double 	theta_r		= -58.5;

    /* Sigmoid gain in mV */
    static constexpr double 	sigma_t		= 6.;
    static constexpr double 	sigma_r		= 6.;

    /* Scaling parameter for sigmoidal mapping (dimensionless) */
    static constexpr double 	C1          = (M_PI/std::sqrt(3));

    /* PSP rise time in ms^-1 */
    static constexpr double 	gamma_e		= 70E-3;
    static constexpr double 	gamma_g		= 100E-3;

    /* Membrane capacitance in muF/cm^2 */
    static constexpr double     C_m			= 1.;

    /* Weights/ conductivities */
    /* Leak  in aU */
    static constexpr double 	g_L    		= 1.;

    /* Synaptic conductivity in ms */
    static constexpr double 	g_AMPA 		= 1.;
    static constexpr double 	g_GABA 		= 1.;

    /* Potassium leak current in mS/m^2 */
    const double                g_LK 		= 0.033;

    /* T current in mS/m^2 */
    static constexpr double     g_T_t		= 3;
    static constexpr double     g_T_r		= 2.3;

    /* h current in mS/m^2 */
    const double                g_h			= 0.02;

    /* Reversal potentials in mV */
    /* Synaptic */
    static constexpr double 	E_AMPA  	= 0.;
    static constexpr double 	E_GABA  	= -70.;

    /* Leak */
    static constexpr double 	E_L_t 		= -70.;
    static constexpr double 	E_L_r 		= -70.;

    /* Potassium */
    static constexpr double 	E_K    		= -100.;

    /* I_T current */
    static constexpr double 	E_Ca    	= 120.;

    /* I_h current */
    static constexpr double 	E_h    		= -40.;

    /* Calcium parameters */
    static constexpr double 	alpha_Ca	= -51.8E-6;			/* influx per spike in nmol		*/
    static constexpr double     tau_Ca		= 10.;				/* calcium time constant in ms	*/
    static constexpr double     Ca_0		= 2.4E-4;			/* resting concentration 		*/

    /* I_h activation parameters */
    static constexpr double 	k1			= 2.5E7;
    static constexpr double 	k2			= 4E-4;
    static constexpr double 	k3			= 1E-1;
    static constexpr double 	k4			= 1E-3;
    static constexpr double 	n_P			= 4.;
    static constexpr double 	g_inc		= 2.;

    /* Noise parameters in ms^-1 */
    static constexpr double 	mphi		= 0E-3;
    static constexpr double     dphi		= 0E-3;
    double                      input		= 0.0;

    /* Connectivities (dimensionless) */
    static constexpr double 	N_tr		= 3.;
    static constexpr double 	N_rt		= 5.;
    static constexpr double 	N_rr		= 25.;

    /* Parameters for SRK4 iteration */
    static constexpr std::array<double,4> A = {0.5, 0.5, 1.0, 1.0};
    static constexpr std::array<double,4> B = {0.75, 0.75, 0.0, 0.0};

    /* Random number generators */
    std::vector<randomStreamNormal> MTRands;

    /* Container for noise */
    std::vector<double>	Rand_vars;

    /* Population variables																			*/
    std::vector<double>	Vt		= init(E_L_t),	/* TC membrane voltage								*/
                        Vr		= init(E_L_r),	/* RE membrane voltage								*/
                        Ca		= init(Ca_0),	/* Calcium concentration of TC population			*/
                        s_et	= init(0.0),	/* PostSP from TC population to TC population		*/
                        s_er	= init(0.0),	/* PostSP from TC population to RE population		*/
                        s_gt	= init(0.0),	/* PostSP from RE population to TC population		*/
                        s_gr	= init(0.0),	/* PostSP from RE population to RE population		*/
                        x_et	= init(0.0),	/* derivative of s_et								*/
                        x_er	= init(0.0),	/* derivative of s_er								*/
                        x_gt	= init(0.0),	/* derivative of s_gt								*/
                        x_gr	= init(0.0),	/* derivative of s_gr								*/
                        h_T_t	= init(0.0),	/* inactivation of T channel						*/
                        h_T_r	= init(0.0),	/* inactivation of T channel						*/
                        m_h		= init(0.0),	/* activation 	of h   channel						*/
                        m_h2	= init(0.0);	/* activation 	of h   channel bound with protein 	*/

    /* Data storage  access */
    friend void get_data (unsigned, Thalamic_Column&, std::vector<double*>&);
};
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/

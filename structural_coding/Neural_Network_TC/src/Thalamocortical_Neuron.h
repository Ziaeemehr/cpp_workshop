#ifndef THALAMOCORTICAL_NEURON_H
#define THALAMOCORTICAL_NEURON_H
#pragma once
#include <cmath>
#include <vector>

#include "Pyramidal_Neuron.h"
#include "Reticular_Neuron.h"

class Inhibitory_Neuron;
class Pyramidal_Neuron;
class Reticular_Neuron;

/******************************************************************************/
/*		Implementation of the thalamocortical neuron after Bazhenov2002       */
/******************************************************************************/
class Thalamocortical_Neuron {
public:
    explicit Thalamocortical_Neuron(const std::vector<double> &Param)
        : E_L(Param[0]), g_L(Param[1]) {}

    /* Set strength of input current */
    void	setInput(double I) {Input = I;}

    /* ODE functions */
    void 	set_RK		(int);
    void 	add_RK	 	(void);
private:
    /* Current functions */
    double 	I_L     (int) const;
    double 	I_LK    (int) const;
    double 	I_Na    (int) const;
    double 	I_K     (int) const;
    double  I_Ca    (int) const;
    double  I_h     (int) const;
    double  I_A     (int) const;

    /* Synaptic currents */
    double 	I_AMPA  (int) const;
    double 	I_NMDA  (int) const;
    double 	I_GABA  (int) const;

    /* Gating functions */
    double 	alpha_h_Na(int) const;
    double 	alpha_m_Na(int) const;
    double 	alpha_n_K (int) const;
    double 	beta_h_Na (int) const;
    double 	beta_m_Na (int) const;
    double 	beta_n_K  (int) const;

    double  m_inf_Ca  (int) const;
    double  h_inf_Ca  (int) const;
    double  m_inf_A   (int) const;
    double  h_inf_A   (int) const;
    double  m_inf_h   (int) const;

    double  tau_m_Ca  (int) const;
    double  tau_h_Ca  (int) const;
    double  tau_m_A   (int) const;
    double  tau_h_A   (int) const;
    double  tau_m_h   (int) const;

    /* Helper functions */
    static void add_RK(std::vector<double>& var) {
        var[0] = (-3*var[0] + 2*var[1] + 4*var[2] + 2*var[3] + var[4])/6;
    }
    static inline std::vector<double> init (double var) {
        return {var, 0.0, 0.0, 0.0, 0.0};
    }

    /* Connections (Neurons that target THIS Neuron*/
    std::vector<Pyramidal_Neuron*> PY_Con;
    std::vector<Reticular_Neuron*> RE_Con;

    /* Paramter constants */
    /* Membrane conductivity */
    const int		C_m		= 1;

    /* Averaged membrane area */
    const double	A_i		= 20E-5;

    /* Reversal potentials */
    const double	E_L		= -63.8;
    const int		E_K		= -90;
    const int		E_Na	= 55;
    const int		E_Ca	= 55;

    const int		E_AMPA	= 0;
    const int		E_NMDA	= 0;
    const int		E_GABA  = -70;

    /* Channel conductivities */
    const double	g_L		= 102.5E-3;
    const double	g_LK	= 102.5E-3;
    const double	g_Na	= 35;
    const double	g_K		= 9;
    const double	g_Ca	= 35;
    const double	g_A		= 9;
    const double	g_h		= 9;

    const double	g_AMPA	= 2.25E-6;
    const double	g_NMDA	= 0.5E-6;
    const double	g_GABA	= 0.165E-6;

    /* Synapse time constants */
    const int		tau_AMPA= 10;
    const int		tau_NMDA= 100;
    const int		tau_x	= 2;

    /* Calcium */
    const double    Ca_0    = 0.1;

    /* Noise parameters */
    const double	dphi	= 5E-3;

    /* Input current */
    double			Input	= 0.0;

    /* Parameters for the RK iteration */
    const std::vector<double> A = {0.5, 0.5, 1.0, 1.0};
    const std::vector<double> B = {0.75, 0.75, 0.0, 0.0};

    /* Variables of the neuron */
    std::vector<double> V		= init(E_L),	/* Dendritic membrane voltage */
                        Ca      = init(Ca_0),   /* Calcium concentration      */
                        h_Na	= init(0.0),	/* inactivation of Na channel */
                        m_Na	= init(0.0),	/* activation   of Na channel */
                        n_K		= init(0.0),    /* activation 	of K  channel */
                        h_Ca	= init(0.0),	/* inactivation of Ca channel */
                        m_Ca	= init(0.0),	/* activation   of Ca channel */
                        h_A     = init(0.0),	/* inactivation of A  channel */
                        m_A 	= init(0.0),	/* activation   of A  channel */
                        m_h		= init(0.0),	/* activation 	of h  channel */
                        m_h2	= init(0.0),    /* activation 	of h  channel bound with protein */
                        s_AMPA	= init(0.0),    /* Fraction of open AMPA channels */
                        s_NMDA	= init(0.0),    /* Fraction of open NMDA channels */
                        x_NMDA	= init(0.0);    /* Derivative of s_NMDA	*/

    /* Other neuron types that recieve input from this neuron type */
    friend class Pyramidal_Neuron;
    friend class Inhibitory_Neuron;
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

#endif // THALAMOCORTICAL_NEURON_H

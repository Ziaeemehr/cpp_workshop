#include "Reticular_Neuron.h"
/******************************************************************************/
/*                              Intrinsic currents                            */
/******************************************************************************/
/* Leak current */
double Reticular_Neuron::I_L	(int N) const{
    return g_L * (V[N] - E_L);
}

/* Potassium leak current */
double Reticular_Neuron::I_LK	(int N) const{
    return g_LK * (V[N] - E_K);
}

/* Fast sodium current */
double Reticular_Neuron::I_Na	(int N) const{
    double am_Na = 0.1*(V[N]+33)/(1-exp(-(V[N]+33)/10));
    double bm_Na = 4*exp(-(V[N]+53.7)/12);
    double m_Na  = am_Na/(am_Na+bm_Na);
    return g_Na * m_Na * m_Na * m_Na * h_Na[N] * (V[N] - E_Na);
}

/* Fast potassium current */
double Reticular_Neuron::I_K	(int N) const{
    return g_K * n_K[N] * n_K[N] * n_K[N] * n_K[N] * (V[N] - E_K);
}

/* Calcium current */
double Reticular_Neuron::I_Ca(int N)  const{
    double m_Ca = 1/(1+exp(-(V[N] + 20)/9));
    return g_Ca * m_Ca * m_Ca * (V[N] - E_Ca);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                              Synaptic currents                             */
/******************************************************************************/
double Reticular_Neuron::I_AMPA(int N)  const{
    double tot_s_AMPA = 0.0;
    for (const auto &syn : PY_Con) {
        tot_s_AMPA += syn->s_AMPA[N];
    }
    for (const auto &syn : TC_Con) {
        tot_s_AMPA += syn->s_AMPA[N];
    }
    return g_AMPA * tot_s_AMPA * (V[N] - E_AMPA);
}

double Reticular_Neuron::I_NMDA(int N)  const{
    double tot_s_NMDA = 0.0;
    for (const auto &syn : PY_Con) {
        tot_s_NMDA += syn->s_NMDA[N];
    }
    for (const auto &syn : TC_Con) {
        tot_s_NMDA += syn->s_NMDA[N];
    }
    return g_NMDA * tot_s_NMDA * (V[N] - E_NMDA);
}

double Reticular_Neuron::I_GABA(int N)  const{
    double tot_s_GABA = 0.0;
    for (const auto &syn : RE_Con) {
        tot_s_GABA += syn->s_GABA[N];
    }
    return g_GABA * tot_s_GABA * (V[N] - E_GABA);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                            Gating functions                                */
/******************************************************************************/
/* Sodium activation */
double Reticular_Neuron::alpha_h_Na(int N) const{
    return 0.128*exp((17 - (V[N] + 50))/18);
}

/* Sodium activation */
double Reticular_Neuron::beta_h_Na(int N) const{
    return 4/(exp((40 - (V[N] + 50))/5) + 1);
}

/* Sodium inactivation */
double Reticular_Neuron::alpha_m_Na(int N) const{
    return 0.32*(13 - (V[N] + 50))/(exp((13 - (V[N] + 50))/4) - 1);
}

/* Sodium inactivation */
double Reticular_Neuron::beta_m_Na(int N) const{
    return 0.28*((V[N] + 50) - 40)/(exp(((V[N] + 50) - 40)/5) - 1);
}

/* Potassium activation */
double Reticular_Neuron::alpha_n_K(int N) const{
    return 0.032*(15 - (V[N] + 50))/(exp((15 - (V[N] + 50))/5) - 1);
}

/* Potassium activation */
double Reticular_Neuron::beta_n_K(int N) const{
    return 0.5*exp((10 - (V[N] + 50))/40);
}

/* Activation of T-type Ca current after Destexhe 1996 */
double Reticular_Neuron::m_inf_Ca	(int N) const{
    double Shift = 2.0;
    return 1.0/(1 + exp(-(V[N] + 50 + Shift)/7.4));
}

/* Inactivation of T-type Ca current after Destexhe 1996 */
double Reticular_Neuron::h_inf_Ca	(int N) const{
    double Shift = 2.0;
    return 1.0/(1+exp((V[N]+78+Shift)/5.));
}

/* Activation time constant of T-type Ca current after Destexhe 1996 */
double Reticular_Neuron::tau_m_Ca	(int N) const{
    return (3.0 + 1.0/(exp((V[N] + 27.)/10.) + exp(-(V[N] + 102.)/15.)))/pow(5.0, 1.2);
}

/* Inactivation time constant of T-type Ca current after Destexhe 1996 */
double Reticular_Neuron::tau_h_Ca	(int N) const{
    return (85.0 + 1.0/(exp((V[N] + 48.)/4.) + exp(-(V[N] + 407.)/50.)))/pow(3.0, 1.2);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                             RK iteration of ODEs 						  */
/******************************************************************************/
void Reticular_Neuron::set_RK(int N) {
    extern const double dt;
    V	  [N+1]=V     [0]+A[N]*dt*(1/C_m *( -(I_L(N) + I_LK(N) + I_Na(N) + I_K(N) + I_Ca(N))
                                            -(I_AMPA(N) + I_NMDA(N) + I_GABA(N))));
    h_Na  [N+1]=h_Na  [0]+A[N]*dt*(alpha_h_Na(N) *(1-h_Na[N]) - beta_h_Na(N) * h_Na[N]);
    m_Na  [N+1]=m_Na  [0]+A[N]*dt*(alpha_m_Na(N) *(1-m_Na[N]) - beta_m_Na(N) * m_Na[N]);
    n_K   [N+1]=n_K   [0]+A[N]*dt*(alpha_n_K (N) *(1-n_K [N]) - beta_n_K (N) * n_K [N]);
    h_Ca  [N+1]=h_Ca  [0]+A[N]*dt*(h_inf_Ca(N) - h_Ca[N])/tau_h_Ca(N);
    m_Ca  [N+1]=m_Ca  [0]+A[N]*dt*(h_inf_Ca(N) - m_Ca[N])/tau_m_Ca(N);
    s_GABA[N+1]=s_GABA[0]+A[N]*dt*(1/(1+exp(-(V[N]-20)/2))*(1-s_GABA[N]) - s_GABA[N]/tau_GABA);
}

void Reticular_Neuron::add_RK(void) {
    add_RK(V);
    add_RK(h_Na);
    add_RK(m_Na);
    add_RK(n_K);
    add_RK(h_Ca);
    add_RK(m_Ca);
    add_RK(s_GABA);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/

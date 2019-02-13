#include "Inhibitory_Neuron.h"
/******************************************************************************/
/*                             Intrinsic currents                             */
/******************************************************************************/
double Inhibitory_Neuron::I_Na(int N)  const{
    double alpha = 0.5*(V[N] + 35) /(1-exp(-(V[N] + 35)/10));
    double beta  = 20*exp(-(V[N] + 60)/18);
    double m_Na  = alpha/(alpha+beta);
    return g_Na * m_Na * m_Na * m_Na * h_Na[N] * (V[N] - E_Na);
}

double Inhibitory_Neuron::I_K(int N)  const{
    return g_K * n_K[N] * n_K[N] * n_K[N] * n_K[N] * (V[N] - E_K);
}

double Inhibitory_Neuron::I_L(int N)  const{
    return g_L * (V[N]- E_L);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                             Gating functions                               */
/******************************************************************************/
double Inhibitory_Neuron::alpha_h_Na(int N)  const{
    return 0.35*exp(-(V[N] + 58)/20);
}

double Inhibitory_Neuron::beta_h_Na(int N)  const{
    return 5/ (1+exp(-(V[N] + 28)/10));
}

double Inhibitory_Neuron::alpha_n_K(int N)  const{
    return 0.05*(V[N] + 34)/(1-exp(-(V[N] + 34)/10));
}

double Inhibitory_Neuron::beta_n_K(int N)  const{
    return 0.625*exp(-(V[N] + 44)/80);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                            Synaptic currents                               */
/******************************************************************************/
double Inhibitory_Neuron::I_AMPA(int N)  const{
    double tot_s_AMPA = 0.0;
    for (const auto &syn : PY_Con) {
        tot_s_AMPA += syn->s_AMPA[N];
    }
    for (const auto &syn : TC_Con) {
        tot_s_AMPA += syn->s_AMPA[N];
    }
    return g_AMPA * tot_s_AMPA * (V[N] - E_AMPA);
}

double Inhibitory_Neuron::I_NMDA(int N)  const{
    double tot_s_NMDA = 0.0;
    for (const auto &syn : PY_Con) {
        tot_s_NMDA += syn->s_NMDA[N];
    }
    for (const auto &syn : TC_Con) {
        tot_s_NMDA += syn->s_NMDA[N];
    }
    return g_NMDA * tot_s_NMDA * (V[N] - E_NMDA);
}

double Inhibitory_Neuron::I_GABA(int N)  const{
    double tot_s_GABA = 0.0;
    for (const auto &syn : IN_Con) {
        tot_s_GABA += syn->s_GABA[N];
    }
    return g_GABA* tot_s_GABA * (V[N] - E_GABA);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                             RK iteration of ODEs 						  */
/******************************************************************************/
void Inhibitory_Neuron::set_RK(int N) {
    extern const double dt;
    V	  [N+1] =V	   [0]+A[N]*dt*(1/C_m *(-(I_L(N) + I_Na(N) + I_K(N))
                                            -(I_AMPA(N) + I_NMDA(N) + I_GABA(N))/A_i));
    h_Na  [N+1] =h_Na  [0]+A[N]*dt*(alpha_h_Na(N) *(1-h_Na[N]) - beta_h_Na(N) * h_Na[N]);
    n_K   [N+1] =n_K   [0]+A[N]*dt*(alpha_n_K (N) *(1-n_K [N]) - beta_n_K (N) * n_K [N]);
    s_GABA[N+1] =s_GABA[0]+A[N]*dt*(1/(1+exp(-(V[N]-20)/2))*(1-s_GABA[N]) - s_GABA[N]/tau_GABA);
}

void Inhibitory_Neuron::add_RK(void) {
    add_RK(V);
    add_RK(h_Na);
    add_RK(n_K);
    add_RK(s_GABA);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


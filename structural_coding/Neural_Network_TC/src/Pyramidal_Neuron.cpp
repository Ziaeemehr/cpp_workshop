#include "Pyramidal_Neuron.h"
/******************************************************************************/
/*                              Intrinsic currents	 						  */
/******************************************************************************/
/* Somatic currents */
/* Leak current */
double Pyramidal_Neuron::I_L	(int N) const{
    return g_L * (Vs[N] - E_L);
}

/* Fast sodium current */
double Pyramidal_Neuron::I_Na	(int N) const{
    double am_Na = 0.1*(Vs[N]+33)/(1-exp(-(Vs[N]+33)/10));
    double bm_Na = 4*exp(-(Vs[N]+53.7)/12);
    double m_Na  = am_Na/(am_Na+bm_Na);
    return g_Na * m_Na * m_Na * m_Na * h_Na[N] * (Vs[N] - E_Na);
}

/* Fast potassium current */
double Pyramidal_Neuron::I_K	(int N) const{
    return g_K * n_K[N] * n_K[N] * n_K[N] * n_K[N] * (Vs[N] - E_K);
}

/* A-type current */
double Pyramidal_Neuron::I_A	(int N) const{
    double m_A	= 1/(1+exp(-(Vs[N]+50)/20));
    return g_A * m_A * m_A * m_A * h_A[N] * (Vs[N] - E_K);
}

/* KS-type current */
double Pyramidal_Neuron::I_KS	(int N) const{
    return g_KS * m_KS[N] * (Vs[N] - E_K);
}

/* Sodium dependent potassium current */
double Pyramidal_Neuron::I_KNa		(int N)  const{
    double w_KNa  = 0.37/(1+pow(38.7/Na[N], 3.5));
    return g_KNa * w_KNa * (Vs[N] - E_K);
}

/* Somato-dendritic leak */
double Pyramidal_Neuron::I_sd	(int N) const{
    return g_sd * (Vs[N] - Vd[N]);
}

/* Dendritic currents */
/* Calcium current */
double Pyramidal_Neuron::I_Ca(int N)  const{
    double m_Ca = 1/(1+exp(-(Vd[N] + 20)/9));
    return g_Ca * m_Ca * m_Ca * (Vd[N] - E_Ca);
}

/* Calcium dependent potassium current */
double Pyramidal_Neuron::I_KCa(int N)  const{
    double m_KCa  = Ca[N]/ (Ca[N] + K_D);
    return g_KCa * m_KCa *  (Vd[N] - E_K);
}

/* Persistent potassium current */
double Pyramidal_Neuron::I_NaP(int N)  const{
    double m_NaP = 1/(1+exp(-(Vd[N]+55.7)/7.7));
    return g_NaP * m_NaP * m_NaP * m_NaP * (Vd[N] - E_Na);
}

/* Inwardly rectifying potassium current */
double Pyramidal_Neuron::I_AR(int N)  const{
    double h_AR  = 1/(1+exp( (Vd[N]+75)/4));
    return g_AR * h_AR * (Vd[N] - E_K);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                              Synaptic currents	 						  */
/******************************************************************************/
double Pyramidal_Neuron::I_AMPA(int N)  const{
    double tot_s_AMPA = 0.0;
    for (const auto &syn : PY_Con) {
        tot_s_AMPA += syn->s_AMPA[N];
    }
    for (const auto &syn : TC_Con) {
        tot_s_AMPA += syn->s_AMPA[N];
    }
    return g_AMPA * tot_s_AMPA * (Vd[N] - E_AMPA);
}

double Pyramidal_Neuron::I_NMDA(int N)  const{
    double tot_s_NMDA = 0.0;
    for (const auto &syn : PY_Con) {
        tot_s_NMDA += syn->s_NMDA[N];
    }
    for (const auto &syn : TC_Con) {
        tot_s_NMDA += syn->s_NMDA[N];
    }
    return g_NMDA * tot_s_NMDA * (Vd[N] - E_NMDA);
}

double Pyramidal_Neuron::I_GABA(int N)  const{
    double tot_s_GABA = 0.0;
    for (const auto &syn : IN_Con) {
        tot_s_GABA += syn->s_GABA[N];
    }
    return g_GABA * tot_s_GABA * (Vd[N] - E_GABA);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                              Gating functions	 						  */
/******************************************************************************/
/* Sodium activation */
double Pyramidal_Neuron::alpha_h_Na(int N) const{
    return 0.28 *exp(-(Vs[N] + 50)/10);
}

/* Sodium activation */
double Pyramidal_Neuron::beta_h_Na(int N) const{
    return 4./(1+exp(-(Vs[N] + 20)/10));
}
/* Potassium activation */
double Pyramidal_Neuron::alpha_n_K(int N) const{
    return 0.04*(Vs[N] + 34)/(1-exp(-(Vs[N] + 34)/10));
}

/* Potassium activation */
double Pyramidal_Neuron::beta_n_K(int N) const{
    return 0.5*exp(-(Vs[N] + 44)/25);
}

/* A_type current inactivation */
double Pyramidal_Neuron::h_A_inf(int N) const{
    return 1/(1+exp( (Vs[N]+80)/6));
}

/* Non-inactivating potassium activation variable */
double Pyramidal_Neuron::m_KS_inf(int N) const{
    return 1/(1+exp(-(Vs[N]+34)/6.5));
}

/* Non-inactivating potassium time constant */
double Pyramidal_Neuron::tau_m_KS(int N) const{
    return 8/(exp( (Vs[N]+55)/30) + exp(-(Vs[N]+55)/30));
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                              Potassium pump	 							  */
/******************************************************************************/
double Pyramidal_Neuron::Na_pump		(int N) const{
    return R_pump*( Na[N]*Na[N]*Na[N]/(Na[N]*Na[N]*Na[N]+3375)
                    -Na_0 *Na_0 *Na_0 /(Na_0 *Na_0 *Na_0 +3375));
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/


/******************************************************************************/
/*                             RK iteration of ODEs 						  */
/******************************************************************************/
void Pyramidal_Neuron::set_RK(int N) {
    extern const double dt;
    Vd	  [N+1]=Vd    [0]+A[N]*dt*(1/C_m *( -(I_Ca(N) + I_KCa (N) + I_NaP(N) + I_AR(N))
                                            -(I_AMPA(N) + I_NMDA(N) - I_sd(N))/A_d));
    Vs	  [N+1]=Vs    [0]+A[N]*dt*(1/C_m *( -(I_L(N) + I_Na(N) + I_K(N) + I_A(N) + I_KS(N)
                                              +I_KNa(N))-(I_GABA(N) + I_sd(N))/A_s));
    Ca    [N+1]=Ca    [0]+A[N]*dt*(-alpha_Ca *  A_d * I_Ca(N) -  Ca[N]/tau_Ca);
    Na    [N+1]=Na    [0]+A[N]*dt*(-alpha_Na *( A_s * I_Na(N) + A_d*I_NaP(N)) - Na_pump(N));
    h_Na  [N+1]=h_Na  [0]+A[N]*dt*(alpha_h_Na(N) *(1-h_Na[N]) - beta_h_Na(N) * h_Na[N]);
    n_K   [N+1]=n_K   [0]+A[N]*dt*(alpha_n_K (N) *(1-n_K [N]) - beta_n_K (N) * n_K [N]);
    h_A   [N+1]=h_A   [0]+A[N]*dt*(h_A_inf(N)  - h_A [N])/tau_A;
    m_KS  [N+1]=m_KS  [0]+A[N]*dt*(m_KS_inf(N) - m_KS[N])/tau_m_KS(N);
    s_AMPA[N+1]=s_AMPA[0]+A[N]*dt*(3.48/(1+exp(-(Vs[N]-20)/2))*(1-s_AMPA[N]) - s_AMPA[N]/tau_AMPA);
    s_NMDA[N+1]=s_NMDA[0]+A[N]*dt*(0.5 * x_NMDA[N] 			  *(1-s_NMDA[N]) - s_NMDA[N]/tau_NMDA);
    x_NMDA[N+1]=x_NMDA[0]+A[N]*dt*(3.48/(1+exp(-(Vs[N]-20)/2))			     - x_NMDA[N]/tau_x);
}

void Pyramidal_Neuron::add_RK(void) {
    add_RK(Vd);
    add_RK(Vs);
    add_RK(Ca);
    add_RK(Na);
    add_RK(h_Na);
    add_RK(h_A);
    add_RK(n_K);
    add_RK(m_KS);
    add_RK(s_AMPA);
    add_RK(s_NMDA);
    add_RK(x_NMDA);
}
/******************************************************************************/
/*                                    end                                     */
/******************************************************************************/

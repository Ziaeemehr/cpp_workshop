#include "functions.h"

/*------------------------------------------------------------*/
StateVec Kuramoto (const StateVec &x) 
{
  double sumx=0;
  int N = PARN;
  StateVec f;
  f = StateVec(N);
  #pragma omp parallel for reduction(+:sumx)
  for (int i=0; i<N; i++){
      sumx = 0;
      for(int j=0; j<N; j++)
          sumx += sin(x[j]-x[i]);
      f[i] = PARw + PARka_over_N * sumx;
  }
  return f;
}
/*------------------------------------------------------------*/
void print_coordinates (double time, const StateVec &y, std::ofstream &output) {
    output <<std::setprecision(10);
    int N = PARN;
    for (int i=0; i<N; i++)
    {
      output <<std::setw(20) << sin(y[i]);
    }
    output<< std::endl;  
}
/*------------------------------------------------------------*/

void euler_integrator (StateVec &y, DerivFunc dydt, double dt) {
  y += dydt (y) * dt;
}

/*------------------------------------------------------------*/
void runge_kutta4_integrator (StateVec &y, DerivFunc dydt, double dt) {
  auto k1 = dydt (y);
  auto k2 = dydt (y + dt*k1/2.);
  auto k3 = dydt (y + dt*k2/2.);
  auto k4 = dydt (y + dt*k3);
  y += (k1 + 2.*k2 + 2.*k3 + k4) * dt/6;
}

/*------------------------------------------------------------*/
void integrate (Integrator integrator, DerivFunc dydt, OutputFunc output_func, 
                std::ofstream &output, StateVec y, int num_steps, double dt) {
  for (auto step = 0; step < num_steps; ++step) {
    output_func (step*dt, y, output);
    integrator (y, dydt, dt);
  }
}
/*------------------------------------------------------------*/
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
/*------------------------------------------------------------*/
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}
/*------------------------------------------------------------*/
void display_timing(double wtime, double cptime)
{
    using namespace std;
    int wh,ch;
    int wmin,cpmin;
    double wsec,csec;
    wh = (int) wtime/3600;
    ch = (int) cptime/3600;
    wmin = ((int)wtime % 3600)/60;
    cpmin = ((int)cptime % 3600)/60;
    wsec = wtime-(3600.*wh+60.*wmin);
    csec = cptime-(3600.*ch+60.*cpmin);
    printf ("Wall Time : %d hours and %d minutes and %.4f seconds.\n",wh,wmin,wsec);
    printf ("CPU  Time : %d hours and %d minutes and %.4f seconds.\n",ch,cpmin,csec);
}
/*------------------------------------------------------------*/
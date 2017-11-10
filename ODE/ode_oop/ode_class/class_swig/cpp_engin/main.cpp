#include "functions.h"

int main()
{
    int N = 2;
    double dt = 0.01;
    std::vector<std::vector<double>> sol;
    std::vector<double> initial_condition {0.0,0.5};
    ODE ode(N, dt, initial_condition);
    sol = ode.integrate(DDE::oscillator,1000);

    for (int i=0; i<sol.size(); i++) {
        for(int j=0; j<sol[0].size(); j++) {
            
            std::cout<<sol[i][j]<<" ";
        }
        std::cout<<"\n";
    }
    return 0;
}
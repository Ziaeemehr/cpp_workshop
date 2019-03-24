#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "functions.h"

class kuramoto_exe
{
    dim2 &adj;
    dim1 &omega;
    double coupling;
    int U;
    int N;

public:
    kuramoto_exe(
        int N_, int U_, double coupling_,
        dim1 &omega_, dim2 &adj_) : adj(adj_), omega(omega_),
                                    U(U_), coupling(coupling_), N(N_) {}

    void operator()(const dim1 &x, dim1 &dxdt, const double /*t*/)
    {
        // define kuramoto model
        for (int i = 0; i < N; i++)
        {
            double sumj = 0.0;
            for (int j = 0; j < N; j++)
            {
                if ((i != j) && (adj[i][j] > 1e-8) && (U == 1))
                    sumj += adj[i][j] * sin(x[j] - x[i]);
                else if ((i != j) && (adj[i][j] > 1e-8) && (U == 0))
                    sumj += adj[i][j] * 0.5* (1 - cos(x[j] - x[i]));
            }
            dxdt[i] = omega[i] + coupling * sumj;
        }
    }
};



// class kuramoto_exe
// {
//     dim2 &adj;
//     dim1 &omega;
//     double coupling;
//     int U;
//     int N;

//   public:
//     kuramoto_exe(
//         int N_, int U_, double coupling_,
//         dim1 &omega_, dim2 &adj_) : adj(adj_), omega(omega_),
//                                     U(U_), coupling(coupling_), N(N_) {}

//     void operator()(const dim1 &x, dim1 &dxdt, const double /*t*/)
//     {
//         // define kuramoto model
//         for (int i = 0; i < N; i++)
//         {
//             double sumj = 0.0;
//             for (int j = 1; j < adj[i].size(); j++) {
//                 int k = adj[i][j];
//                 if (U == 1) {
//                     sumj += sin(x[k] - x[i]); 
//                 }
//                 else if (U == 0) {
//                     sumj += (1 - cos(x[k] - x[i])); 
//                 }
//                 else {
//                 cout << "undefined type of oscillator \n";
//                 exit(EXIT_FAILURE);
//                 }
//             }
//         dxdt[i] = omega[i] + coupling * sumj;
//         }
//     }
// };

#endif
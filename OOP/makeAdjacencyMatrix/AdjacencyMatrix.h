#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

using namespace std;

using dim1 = vector<double>;
using dim2 = vector<dim1>;

constexpr int N = 28;


class AdjacencyMatrix
{
    private:
        int Node,neighbour,n_comm;
        dim2 AdjMatrix;
        std::string Type;
        double prob,pOut,pIn;        
    public:
        AdjacencyMatrix(){ }
        AdjacencyMatrix(int iN, std::string input, double ip,
        double ipIn, double ipOut, int in_comm, int ineighb)
        {
            Node = iN;
            Type = input; 
            prob = ip;
            pIn = ipIn; 
            pOut = ipOut;
            n_comm = in_comm;
            neighbour = ineighb;
        }
        dim2 get_adjacency_matrix();
        dim2 ErdosRenyiNet(double prob);
        dim2 allToAll();
        dim2 Modular(double pIn, double pOut, int n_comm);
};

#endif

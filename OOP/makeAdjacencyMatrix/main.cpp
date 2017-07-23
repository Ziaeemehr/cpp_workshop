#include "AdjacencyMatrix.h"

int main()

{   
    // N, type, p, pIn, pOut, n_comm, neighbour
    // AdjacencyMatrix A1(N,"ER",0.1,0,0,0,0);
    // AdjacencyMatrix A1(N,"ALL",0.,0,0,0,0);
    AdjacencyMatrix A1(N,"MOD",0.0,1.0,0.1,4,0);
    auto Adj = A1.get_adjacency_matrix();
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cout << Adj[i][j]<<" ";
        }
        cout <<"\n";
    }
    return 0;
}

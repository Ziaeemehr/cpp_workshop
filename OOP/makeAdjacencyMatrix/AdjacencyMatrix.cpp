#include "AdjacencyMatrix.h"

dim2 AdjacencyMatrix::get_adjacency_matrix() 
{
    if (Type=="ER") 
        AdjMatrix = ErdosRenyiNet(prob);
    else if (Type == "ALL") 
        AdjMatrix = allToAll();
    else if (Type =="MOD") 
        AdjMatrix = Modular(pIn, pOut, n_comm);
    else {
        std::cerr << "\n Enter a valid network type \n";
        std::cerr << " Erdos Renyi: (ER), all To all : (ALL) ";
        std::cerr << " Modular : (MOD) \n ";
        exit(2);
    }
    return AdjMatrix;
}

dim2 AdjacencyMatrix::ErdosRenyiNet(double prob)
{
    //creat Erdős–Rényi network and return it's adjacency MATrix

    dim2 Cij(Node,dim1(Node));
    for (int i=0; i<Node; i++) {
        for (int j=i; j<Node; j++) {
            double r= (double)rand()/RAND_MAX;
            if ( (r < prob) && (i!=j) ) {
                Cij[i][j] = Cij[j][i] = 1;
            }
        }
    }
    
    return Cij;
}

dim2 AdjacencyMatrix::allToAll()
{
    dim2 Cij(Node,dim1(Node));
    for (int i=0; i<Node; i++)
        for (int j=i+1; j<Node; j++)
            Cij[i][j] = Cij[j][i]= 1.0;
    return Cij;
}

dim2 AdjacencyMatrix::Modular(double pIn ,double pOut, int n_comm )
{
    //creat Modular network and return it's adjacency MATrix
    /*
     * N  vertices
     * n_com number of community
     * len size of community
     * pIn probability of being a connection between two nodes
       inside a community 
       pOut probability of being a connection between two nodes
       between communities
     */
     cout <<pIn<<" " <<pOut<<endl;
    dim2 Cij(Node,dim1(Node));
    int len = (int)(Node/n_comm);

    for (int i=0; i<Node; i++) {
        for(int j=i+1; j<Node; j++) {
            double r = (double)rand()/(RAND_MAX);
            if (r < pOut) {
                Cij[i][j] = Cij[j][i] = 1.0;
            }
        }
    }

    int s=0;
    for (int k=0; k<n_comm; k++){
        if (k>0) s += len;
        for (int i=s; i<(len+s); i++){
            for(int j=i+1; j<(len+s); j++){
                double r = (double)rand()/(RAND_MAX);
                if (r < pIn)
                    Cij[i][j] = Cij[j][i] = 1.0;
            }
        }
    }
    
    return Cij;
}

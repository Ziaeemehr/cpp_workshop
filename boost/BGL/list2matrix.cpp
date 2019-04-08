    int m = 4; // number of nodes
    int edges = 4; // number if items in the list
    int list[edges][2] = { {0,1}, {0,2}, {0,3}, {2,3} }; // adjacency list
    int mat[m][m] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} }; // empty matrix
     
    for(int i=0;i<edges;++i) {
      int a = list[i][0];  // first vertex
      int b= list[i][1];   // second vertex
      mat[a][b] = 1;  // set item in the matrix
      mat[b][a] = 1; // may want to set the symmetrical element as well
    }

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/random/linear_congruential.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef small_world_iterator<boost::minstd_rand, Graph> SWGen;

int main()
{
    
    boost::minstd_rand gen;
    int N = 20;
    int degree = 4;
    double rewiring = 0.;
    
    Graph g(SWGen(gen, N, degree, rewiring), SWGen(), 20);
  
    cout << num_edges(g)<< '\n';
    
    typedef graph_traits<Graph>::edge_iterator edge_iterator;
    pair<edge_iterator, edge_iterator> ei = edges(g);
    
    for(edge_iterator edge_iter = ei.first; edge_iter != ei.second; ++edge_iter) {
        cout << "(" << source(*edge_iter, g) << ", " << target(*edge_iter, g) << ")\n";
    }
    vector<vector<int> > mat(N,vector<int>(N));
        
    for (edge_iterator edge_iter = ei.first; edge_iter != ei.second; ++edge_iter){
        int a = source(*edge_iter, g);
        int b = target(*edge_iter, g);
        mat[a][b] = 1;
        mat[b][a] = 1;
    }
    
    
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            cout << mat[i][j]<<" ";
        }
        cout <<endl;
    }
  
  return 0;
}

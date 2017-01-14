#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>

using namespace boost;

typedef boost::adjacency_list< listS, vecS, directedS > ListGraph;
typedef boost::adjacency_matrix< directedS > MatrixGraph;

int main(){ 
    
    ListGraph lg; 
    add_edge (0, 1, lg); 
    add_edge (0, 3, lg); 
    add_edge (1, 2, lg); 
    add_edge (2, 3, lg); 
    
    //How do I get the adjacency matrix underlying lg?
    
    MatrixGraph mg(3); 
    add_edge (0, 1, mg); 
    add_edge (0, 3, mg); 
    add_edge (1, 2, mg); 
    add_edge (2, 3, mg); 
    
    //How do I get the adjacency matrix underlying mg?
    
} 

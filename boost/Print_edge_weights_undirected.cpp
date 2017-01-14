#include <boost/graph/adjacency_list.hpp>
#include <iostream>
using namespace std;
using namespace boost;

typedef property<edge_weight_t, double> EdgeWeight;
typedef adjacency_list<listS, vecS,undirectedS,no_property,EdgeWeight> UndirectedGraph;
typedef graph_traits<UndirectedGraph>::edge_iterator edge_iterator;

int main() //int,char*[]
{
    // Undirected graph - print out the edge weights
    UndirectedGraph g;
    
    add_edge (0, 1, 8, g);
    add_edge (0, 5, 2, g);
    add_edge (2, 3, 2, g);
    add_edge (1, 3, 7, g);
    add_edge (1, 4, 1, g);
    add_edge (4, 5, 6, g);
    add_edge (2, 5, 7, g);
    
    property_map<UndirectedGraph,edge_weight_t> :: type EdgeWeightMap;
    EdgeWeightMap = get(edge_weight_t(),g);
    
    pair<edge_iterator,edge_iterator> edgePair;
    edgePair = edges(g);
    
    cout << "Number of edges = " << num_edges(g) << endl;
    cout << "(Edge list) and weights \n";
    for (edge_iterator it = edgePair.first; it != edgePair.second; ++it)
    {
	cout << *it <<"   "<< EdgeWeightMap[*it] << endl;
    }
    cout << endl;
    return 0;
}

#include<boost/graph/adjacency_list.hpp>
#include<iostream>

using namespace std;
using namespace boost;
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef adjacency_list<listS, vecS,directedS, no_property, EdgeWeightProperty > DirectedGraph;
typedef graph_traits<DirectedGraph>::edge_iterator edge_iterator;

int main()
{
    DirectedGraph g;
    add_edge(0,1,8,g);
    add_edge (0, 3, 18, g);
    add_edge (1, 2, 20, g);
    add_edge (2, 3, 2, g);
    add_edge (3, 1, 1, g);
    add_edge (1, 3, 7, g);
    add_edge (1, 4, 1, g);
    add_edge (4, 5, 6, g);
    add_edge (2, 5, 7, g);
    pair <edge_iterator,edge_iterator> ei = edges(g);
    cout << "Number of edges = " << num_edges(g) << "\n";
    cout << "Edge list : \n";
    copy (ei.first, ei.second,ostream_iterator<adjacency_list<>::edge_descriptor>{
	cout, "\n"});
    cout << endl;
    return 0;
}
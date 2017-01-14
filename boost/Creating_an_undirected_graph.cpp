#include <boost/graph/adjacency_list.hpp>
#include <iostream>
using namespace std;


typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS,boost::undirectedS,boost::no_property,EdgeWeightProperty> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator vertex_iterator;
using adjacency_iterator = boost::graph_traits<UndirectedGraph>::adjacency_iterator;

int main()
{
    UndirectedGraph g;
    boost::add_edge(0,1,8,g);
    boost::add_edge (0, 3, 18, g);
    boost::add_edge (1, 2, 20, g);
    boost::add_edge (2, 3, 2, g);
    boost::add_edge (1, 3, 7, g);
    boost::add_edge (1, 4, 1, g);
    boost::add_edge (4, 5, 6, g);
    boost::add_edge (2, 5, 7, g);
    pair<vertex_iterator,vertex_iterator> ei = vertices(g);
    cout << "Number of edges = " << num_edges(g) << endl;
    cout << "Edge list : \n";
    for (vertex_iterator it = ei.first; it != ei.second; ++it)
    {
	cout << *it << '\t';
	pair <adjacency_iterator, adjacency_iterator> ai = adjacent_vertices (*it, g);
	for (adjacency_iterator it2 = ai.first; it2 != ai.second; ++it2) cout << *it2 << ' ';
	cout << '\t' << boost::degree (*it, g) << endl;
    }
    cout << endl;
    return 0;
}

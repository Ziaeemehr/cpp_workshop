#include<boost/config.hpp>
#include <iostream>
#include<fstream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
//using namespace boost;
int main(int, char *[])
{
    //typedef boost::property<boost::edge_weight_t, int>  EdgeWeight;
    typedef boost::adjacency_list<boost::listS, boost::vecS,boost::directedS,
    boost::no_property,boost::property<boost::edge_weight_t, int> > graph_t;
    typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
    typedef pair<int, int> Edge;
    const int num_nodes = 5;
    enum nodes {A, B, C, D, E};
    char name[] = "ABCDE";
    Edge edge_array[] = { Edge(A,C), Edge(B,B), Edge(B, D), Edge(B, E),
        Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
        };
    int weights[] = { 1,2,1,2,7,3,1,1,1 };
    int num_arcs = sizeof(edge_array)/sizeof(Edge);
       
    // Graph created from the list of edges
    graph_t g(edge_array, edge_array+num_arcs, weights, num_nodes);

    // Create property_map from edges to weights
    boost::property_map<graph_t, boost::edge_weight_t>:: type weightmap = get(boost::edge_weight,g);
    
    // Create vectors to store the predecessors (p) and the distances from the root (d)
    vector<vertex_descriptor> p(num_vertices(g));
    vector<int> d(num_vertices(g));
    
    //Creat descriptor for the source node
    vertex_descriptor s = vertex(A,g);
    vertex_descriptor goal = vertex(E,g);
    
    // Evaluate Dijkstra on graph g with source s, predecessor_map p and distance_map d
    boost::dijkstra_shortest_paths(g, s, 
	boost::predecessor_map(&p[0]).distance_map(&d[0]));
    //p[] is the predecessor map obtained through dijkstra
    //name[] is a vector with the names of the vertices
    //s and goal are vertex descriptors
    vector<boost::graph_traits<graph_t>::vertex_descriptor > path;
    boost::graph_traits<graph_t>::vertex_descriptor current = goal;

    while(current!=s) 
    {
	path.push_back(current);
	current = p[current];
    }
    path.push_back(s);
    //Prints the path obtained in reverse
    cout << "Path from " << name[s] << " to " << name[goal] << endl;
    vector<boost::graph_traits<graph_t>::vertex_descriptor >::reverse_iterator it;
    
    for (it = path.rbegin(); it != path.rend(); it++){
	cout << name[*it] << " ";
    }
    cout << endl;
    return EXIT_SUCCESS;
}
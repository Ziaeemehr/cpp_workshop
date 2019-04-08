#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/copy.hpp>
#include <cassert>

using namespace boost;

typedef boost::adjacency_list< listS, vecS, directedS > ListGraph;
typedef boost::adjacency_matrix< directedS > MatrixGraph;

template <class Graph>
class MatrixAccessor
{
public:
    typedef typename Graph::Matrix Matrix; //actually a vector<
    typedef typename Matrix::const_reference const_reference;


    MatrixAccessor(const Graph* g)
        : m_g(g)
    {
        static_assert(boost::is_same<size_t, typename Graph::vertex_descriptor>::value, "Vertex descriptor should be of integer type");
    }

    const_reference operator()(size_t u, size_t v) const
    {
        return m_g->get_edge(u, v);
    }

    const Graph* m_g;
};

void use_matrix(const MatrixGraph & mg)
{
    MatrixAccessor<MatrixGraph> matr(&mg);
    assert(matr(0, 1) == 1);
    assert(matr(0, 2) == 0);
}


int main(){

    ListGraph lg;
    add_edge(0, 1, lg);
    add_edge(0, 3, lg);
    add_edge(1, 2, lg);
    add_edge(2, 3, lg);

    //How do I get the adjacency matrix underlying lg?

    //How do I get the adjacency matrix underlying mg?   
    MatrixGraph mg( num_vertices(lg));
    boost::copy_graph(lg, mg);
}

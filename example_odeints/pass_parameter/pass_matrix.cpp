#include <iostream>
#include <vector>
#include <boost/numeric/odeint.hpp>

using namespace std;
/* The type of container used to hold the state vector */
typedef vector< double > state_type;

/* The rhs of x' = f(x) defined as a class */
class harm_osc {
    vector<state_type>& m_gam;
public:
    harm_osc( vector<state_type>& gam ) : m_gam(gam) { }

    void operator() ( const state_type &x , state_type &dxdt , const double /* t */ )
    {
        dxdt[0] = m_gam[0][0]*x[1];
        dxdt[1] = m_gam[1][0]*x[0] - m_gam[1][1]*x[1];
    }
};
//to see the result in every time step:
struct push_back_state_and_time
{
    vector< state_type >& m_states;
    vector< double >& m_times;

    push_back_state_and_time( vector< state_type > &states , vector< double > &times )
    : m_states( states ) , m_times( times ) { }

    void operator()( const state_type &x , double t )
    {
        m_states.push_back( x );
        m_times.push_back( t );
    }
};


int main(int /* argc */ , char** /* argv */ )
{
    using namespace boost::numeric::odeint;


    //[ state_initialization
    state_type x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;
    //]

    vector<state_type> x_vec;
    vector<double> times;

    //[ integration_class
    vector<state_type> par {{1.,0},
                            {-1,0.15}};
    harm_osc ho(par);
    size_t steps = integrate( ho ,
            x , 0.0 , 10.0 , 0.1,
            push_back_state_and_time( x_vec , times ));
    //]

    /* output */
    for( size_t i=0; i<=steps; i++ )
    {
        cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    }
}
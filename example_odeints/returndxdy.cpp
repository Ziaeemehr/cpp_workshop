#include <iostream>
#include <vector>
#include <cmath>

#include <boost/numeric/odeint.hpp>
using namespace std;
/* The type of container used to hold the state vector */
typedef vector< double > state_type;

const double gam = 0.15;

/* The rhs of x' = f(x) */
void harmonic_oscillator( const state_type &x , state_type &dxdt , const double /* t */ )
{
    state_type dx(2);
    auto func = [&](double c1) 
    {
        return c1;
    };
    dx[0] = x[1]*func(1.);
    dx[1] = -x[0] - gam*x[1];
    dxdt = dx;
}


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
    
    runge_kutta4< state_type > stepper;
    size_t steps = integrate_const( stepper , harmonic_oscillator , 
                                    x , 0.0 , 10.0 , 0.01,
                                    push_back_state_and_time( x_vec , times ) );

//     size_t steps = integrate( harmonic_oscillator ,
//             x , 0.0 , 10.0 , 0.001 ,
//             push_back_state_and_time( x_vec , times ) );

    /* output */
    for( size_t i=0; i<=steps; i++ )
    {
        cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    }
}
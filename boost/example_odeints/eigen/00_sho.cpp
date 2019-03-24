#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <boost/numeric/odeint.hpp>

// using namespace std;
using namespace boost::numeric::odeint;
using namespace Eigen;

/*-----------------------------------------------------------------*/
/* rhs_function, The type of container used to hold the state vector */
// typedef std::vector< double > state_type;
template <size_t N>
using vector = Matrix<double, N, 1>;
typedef vector<3> state_type;


const double gam = 0.05;

/* The rhs of x' = f(x) */
void harmonic_oscillator( const state_type &x , state_type &dxdt , const double /* t */ )
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}
/*-----------------------------------------------------------------*/
// struct push_back_state_and_time
// {
//     std::vector< state_type >& m_states;
//     std::vector< double >& m_times;

//     push_back_state_and_time( std::vector< state_type > &states , std::vector< double > &times )
//     : m_states( states ) , m_times( times ) { }

//     void operator()( const state_type &x , double t )
//     {
//         m_states.push_back( x );
//         m_times.push_back( t );
//     }
// };
/*-----------------------------------------------------------------*/
int main(int /* argc */ , char** /* argv */ )
{
    // state_initialization
    state_type x(2);
    x[0] = 1.0; 
    x[1] = 0.0;
    
    // vector<state_type> x_vec;
    // vector<double> times;
    
    
     // integration
    size_t steps = integrate( harmonic_oscillator ,
            x , 0.0 , 100.0 , 0.1);//, push_back_state_and_time( x_vec , times ) );

    
    // for( size_t i=0; i<=steps; i++ ) {
    //     cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    // }
}
/*-----------------------------------------------------------------*/
// typedef runge_kutta_cash_karp54< state_type > error_stepper_type;
// typedef controlled_runge_kutta< error_stepper_type > controlled_stepper_type;
// controlled_stepper_type controlled_stepper;
// integrate_adaptive( controlled_stepper , harmonic_oscillator , x , 0.0 , 10.0 , 0.01 );

// struct write_state
// {
//     void operator()( const state_type &x ) const
//     {
//         std::cout << x[0] << "\t" << x[1] << "\n";
//     }
// };

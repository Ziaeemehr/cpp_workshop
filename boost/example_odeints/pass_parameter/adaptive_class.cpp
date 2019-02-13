#include <iostream>
#include <vector>

#include <boost/numeric/odeint.hpp>

/* The type of container used to hold the state vector */
typedef std::vector< double > state_type;

/* The rhs of x' = f(x) defined as a class */
class harm_osc {
    double m_gam;
public:
    harm_osc( double gam ) : m_gam(gam) { }

    void operator() ( const state_type &x , state_type &dxdt , const double /* t */ )
    {
        dxdt[0] = x[1];
        dxdt[1] = -x[0] - m_gam*x[1];
    }
};


struct push_back_state_and_time
{
    std::vector< state_type >& m_states;
    std::vector< double >& m_times;

    push_back_state_and_time( std::vector< state_type > &states , std::vector< double > &times )
    : m_states( states ) , m_times( times ) { }

    void operator()( const state_type &x , double t )
    {
        m_states.push_back( x );
        m_times.push_back( t );
    }
};


int main(int /* argc */ , char** /* argv */ )
{
    using namespace std;
    using namespace boost::numeric::odeint;


    //[ state_initialization
    state_type x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;
    //]
    
    vector<state_type> x_vec;
    vector<double> times;
       
    typedef runge_kutta_cash_karp54< state_type > error_stepper_type;
    
    typedef controlled_runge_kutta< error_stepper_type > controlled_stepper_type;
    controlled_stepper_type controlled_stepper;
    
//     harm_osc ho(0.15);
    size_t steps = integrate_adaptive( make_controlled< error_stepper_type >( 1.0e-10 , 1.0e-6 ) , 
                    harm_osc{0.15} , x , 0.0 , 10.0 , 0.01,
                    push_back_state_and_time( x_vec , times ) );
//     size_t steps = integrate_adaptive( controlled_stepper , harmonic_oscillator , 
//                         x , 0.0 , 10.0 , 0.01,
//                         push_back_state_and_time( x_vec , times ) );
    cout << "steps: " << steps << endl;
    /* output */
    for( size_t i=0; i<=steps; i++ )
    {
        cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    }
}


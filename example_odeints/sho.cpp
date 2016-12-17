#include <iostream>
#include <vector>

#include <boost/numeric/odeint.hpp>



//[ rhs_function
/* The type of container used to hold the state vector */
typedef std::vector< double > state_type;

const double gam = 0.15;

/* The rhs of x' = f(x) */
void harmonic_oscillator( const state_type &x , state_type &dxdt , const double /* t */ )
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}
//]


struct write_state
{
    void operator()( const state_type &x ) const
    {
        std::cout << x[0] << "\t" << x[1] << "\n";
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
    
     //[ integration
    size_t steps = integrate( harmonic_oscillator ,
            x , 0.0 , 10.0 , 0.1 );
    //]
    for( size_t i=0; i<=steps; i++ )
    {
//         cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
        cout << x[0]<< '\t' << x[1] << endl;
    }
}

// typedef runge_kutta_cash_karp54< state_type > error_stepper_type;
// 
// typedef controlled_runge_kutta< error_stepper_type > controlled_stepper_type;
// controlled_stepper_type controlled_stepper;
// integrate_adaptive( controlled_stepper , harmonic_oscillator , x , 0.0 , 10.0 , 0.01 );
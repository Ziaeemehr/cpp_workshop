#include <iostream>
#include <vector>

#include <boost/numeric/odeint.hpp>

/*-----------------------------------------------------------------*/
/* rhs_function, The type of container used to hold the state vector */
typedef std::vector<double> state_type;

const double gam = 0.05;

/* The rhs of x' = f(x) */
void harmonic_oscillator(const state_type &x, state_type &dxdt, const double /* t */)
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam * x[1];
}
/*-----------------------------------------------------------------*/
struct push_back_state_and_time
{
    std::vector<state_type> &m_states;
    std::vector<double> &m_times;

    push_back_state_and_time(std::vector<state_type> &states, std::vector<double> &times)
        : m_states(states), m_times(times) {}

    void operator()(const state_type &x, double t)
    {
        m_states.push_back(x);
        m_times.push_back(t);
    }
};
/*-----------------------------------------------------------------*/
int main(int /* argc */, char ** /* argv */)
{
    using namespace std;
    using namespace boost::numeric::odeint;

    // state_initialization
    state_type x(2);
    x[0] = 1.0;
    x[1] = 0.0;
    double dt = 0.01;
    int nsteps = 50;
    vector<state_type> x_vec;
    vector<double> times;


    euler<state_type> stepper;
    // runge_kutta4<state_type> stepper;
    // runge_kutta4<state_type, double, state_type, double, range_algebra> rk4;

    // integration
    // size_t steps = integrate( harmonic_oscillator ,
    //         x , 0.0 , 100.0 , 0.1, push_back_state_and_time( x_vec , times ) );

    integrate_n_steps(stepper, harmonic_oscillator, x, 0.0, dt, nsteps,
                      push_back_state_and_time(x_vec, times));

    for (size_t i = 0; i < nsteps; i++)
    {
        cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    }
}
/*-----------------------------------------------------------------*/
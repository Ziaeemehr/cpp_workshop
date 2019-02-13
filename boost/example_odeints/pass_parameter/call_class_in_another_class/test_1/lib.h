#include <iostream>
#include <vector>
#include <utility>
#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;

/* The type of container used to hold the state vector */
typedef vector<double> state_type;
/*------------------------------------------------------------*/
struct push_back_state_and_time
{
    vector<state_type> &m_states;
    vector<double> &m_times;

    push_back_state_and_time(vector<state_type> &states, vector<double> &times)
        : m_states(states), m_times(times) {}

    void operator()(const state_type &x, double t)
    {
        m_states.push_back(x);
        m_times.push_back(t);
    }
};
/*------------------------------------------------------------*/
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
/*------------------------------------------------------------*/
template <class HarmType>
class osc_solver {
public:
     template <class... ArgsType>
     osc_solver(ArgsType&&... parameters_harm) : m_ho(std::forward<ArgsType>(parameters_harm)...) {
        x = {1.0, 0.0}; // start at x=1.0, p=0.0

    }
    void run()
    {
        // harm_osc ho(0.15);
        // ho(0.15);
        size_t steps = integrate(m_ho,
                                 x, 0.0, 10.0, 0.1,
                                 push_back_state_and_time(x_vec, times));

        /* output */
        for (size_t i = 0; i <= steps; i++)
        {
            cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
        }
    }

  private:
    HarmType m_ho;
    state_type x;
    vector<state_type> x_vec;
    vector<double> times;
};


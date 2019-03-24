#include <Eigen/Core>
#include <boost/numeric/odeint/stepper/runge_kutta_dopri5.hpp>
#include <iostream>

using namespace Eigen;
using namespace boost::numeric::odeint;

template<size_t N>
using vector = Matrix<double, N, 1>;

typedef vector<3> state;

int main() {

    state X0;
    X0 << 1., 2., 3.;
    state xout = X0;

    runge_kutta_dopri5<state, double, state, double, vector_space_algebra> stepper;

    stepper.do_step([](const state &x, state &dxdt, const double t) -> void {
        dxdt = x;
    },X0, 0.0, xout, 0.01);

    std::cout << xout << std::endl;
}
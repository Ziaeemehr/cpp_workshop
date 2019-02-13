#include "lib.h"

void osc_solver::run()
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
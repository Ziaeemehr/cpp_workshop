#include "lib.h"



int main(int /* argc */ , char** /* argv */ )
{
    osc_solver<harm_osc> solver(0.15);
    solver.run();
    return 0;
}

#include "lib.h"



int main(int /* argc */ , char** /* argv */ )
{
    osc_solver sol(harm_osc(0.15));
    sol.run();
    return 0;
}

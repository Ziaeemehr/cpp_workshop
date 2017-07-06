#include "functions.h"

#define NUM_POINTS 5

// namespace alglib{
// void alglib::corrr1d(
//     real_1d_array signal,
//     ae_int_t n,
//     real_1d_array pattern,
//     ae_int_t m,
//     real_1d_array& r);
// }
int main()
{
    ae_int_t n{NUM_POINTS};
    ae_int_t m{NUM_POINTS};
    real_1d_array signal = "[0.0,0.1,0.2,0.3,0.4]";
    real_1d_array pattern = "[0.00,0.05,0.26,0.32,0.33]";
    real_1d_array r ;//= "[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]"; 
    
    //create 2 sample signals
//     dim1 t_steps = linspace(0,1,n);
    
    corrr1d(signal, n, pattern,m, r);

    // print output
    for (int i = NUM_POINTS; i<2*NUM_POINTS-1; i++)
        std::cout << r[i]<<std::endl;
    
    for (int i=0; i<NUM_POINTS; i++)
        std::cout << r[i]<<std::endl;
    
    return 0;
}


//     for (int i=0; i<n; i++) {
//         a_sig[i] = sin(2*M_PI*t_steps[i]*4+5);
//         b_sig[i] = sin(2*M_PI*t_steps[i]*4);
//     }

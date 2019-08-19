#include "lib.h"

int main()
{

    vector<double> A ={ 1.0, 1.0,-2.0, 
                        1.0, 2.0, -3.0, 
                        0.0, 1.0, 1.0};
    // vector<double> A = {1.0, 1.0, -2.0, 1.0, 
    //                     1.0, 2.0, -3.0, 0.0,
    //                     0.0, 1.0,  1.0, 2.0,  
    //                     1.0, -5.0, 2.0, 0.0};
    
    dim1 znorm(3);
    gram_schmidt(A, 3, znorm);
    // gram_schmidt_2(A, znorm);

    return 0;
}

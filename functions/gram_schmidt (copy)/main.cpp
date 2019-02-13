#include "lib.h"

void gram_schmidt(dim1& A, const int n, dim1& znorm) 
{    
    dim1 B(n*n);
    auto first_A = A.begin();
    auto first_B = B.begin();

    // iterator beg1 = first, end1 = first + n;
    dim1 znorm(n);
    std::fill(znorm.begin(), znorm.end(), 0.0);
    
    dim1 bp(first_A, first_A+n);
    znorm[0] = norm(bp);
    
    for (int i=0; i<n; i++)
        B[i] = bp[i]/(znorm[0]+0.0);

    for (int k=1; k<n; k++) {
        dim1 sumj(n);
        for (int j=0; j<k; j++) {
            dim1 a(first_A+(k*n), first_A+(k+1)*n);
            dim1 b(first_B+(j*n), first_B+((j+1)*n));
            sumj = sumj + proj(a,b);
        }
        
        for (int i=0; i<n; i++)
            bp[i] = A[k*n+i] - sumj[i];

        znorm[k] = norm(bp);
        for (int i=0; i<n; i++)
            B[k*n+i] = bp[i]/znorm[k];
    }

    dim1 a1(n);
    dim1 a2(n);
    dim1 a3(n);
    for (int i=0; i<n; i++) {
        a1[i] = B[i];
        a2[i] = B[n+i];
        a3[i] = B[2*n+i];
    }

    double s = 0.0;
    cout << dot(a1, a2) << endl;
    cout << dot(a1, a3) << endl;
    cout << dot(a2, a3) << endl;

    for (int i=0; i<n; i++)
        cout << znorm[i] << " ";
    cout << endl;
}



int main()
{

    vector<double> A ={ 1.0, 1.0,-2.0, 
                        1.0, 2.0, -3.0, 
                        0.0, 1.0, 1.0};
    // vector<double> A = {1.0, 1.0, -2.0, 1.0, 
    //                     1.0, 2.0, -3.0, 0.0,
    //                     0.0, 1.0,  1.0, 2.0,  
    //                     1.0, -5.0, 2.0, 0.0};

    gram_schmidt(A, 3);

    return 0;
}

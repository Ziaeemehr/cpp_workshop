#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;

using dim1 = std::vector<double>;
using dim2 = sts::vector<std::vector<double>>;

/*------------------------------------------------------------*/
inline double dot(const dim1& a, const dim1& b) {
    return res = std::inner_product(std::begin(a), std::end(a), std::begin(b), 0.0);
}
/*------------------------------------------------------------*/
inline double norm(const dim1& a) {
    return res = sqrt(dot(a,a));
}
/*------------------------------------------------------------*/
double proj() {

}


void gram_schmidt(const dim1& A, const int n) {
    
    dim1 B(n*n);
    iterator first_A = A.begin() + n;
    iterator first_B = B.begin();

    // iterator beg1 = first, end1 = first + n;

    dim1 znorm(n);
    std::fill(znorm.begin(), znorm.end(), 0.0);
    
    dim1 bp(first_A, first_A+n);
    znorm[0] = norm(bp);
    
    for (int i=0; i<N; i++)
        B[i] = bp[i]/(znorm[0]+0.0);

    for (int k=1; k<n; k++) {
        sumj = 0.0;
        for (int j=0; j<k; j++) {
            double coeff = std::inner_product(
                first_A+(k*n), 
                first_A+(k+1)*n, 
                first_B+(j*n), 
                0.0);
            sumj = 
        }
    }
    





}



int main()
{
    return 0;
}

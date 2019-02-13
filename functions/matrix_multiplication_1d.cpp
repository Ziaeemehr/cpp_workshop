#include <vector>
#include <iostream>

using namespace std;


std::vector<int> multiply_matrices(
            std::vector<int> matA,
            std::vector<int> matB,
            int rA, int cA, 
            int rB, int cB, 
            int rC, int cC) 
{
    vector<int> matC(rC*cC);
    for (int i = 0; i < rA; i++) {
        for (int j = 0; j < cB; j++) {
            float sum = 0.0;
            for (int k = 0; k < rB; k++)
                sum = sum + matA[i * cA + k] * matB[k * cB + j];
            matC[i * cC + j] = sum;
        }

    }
    return matC;
}

int main() 
{
    vector<int> A = {1, 2, -1, 2, 0, 1};
    vector<int> B = {3,1, 0,-1,-2, 3};
    auto C = multiply_matrices(A, B, 
                               2, 3,
                               3, 2,
                               2, 2);
    
}


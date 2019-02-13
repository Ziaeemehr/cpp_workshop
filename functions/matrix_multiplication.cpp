#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
using namespace std;

using dim2 = std::vector<std::vector<int>>;
using dim1 = std::vector<int>;


dim2 multiplyMatrices( dim2 &A, dim2 &B)
{
    int r1 = A.size();
    int c1=A[0].size();
    int r2 = B.size();
    int c2 = B[0].size();
    
//     asser(c1==r2); // && "bad dimension for matrix multiplication");
    
    dim2 mult(r1, std::vector<int>(c2));

	// Initializing elements of matrix mult to 0.
	for(int i = 0; i < r1; ++i) {
		for(int j = 0; j < c2; ++j) {
			mult[i][j] = 0;
		}
	}

	// Multiplying matrix A and B and storing in array mult.
	for(int i = 0; i < r1; ++i) {
		for(int j = 0; j < c2; ++j) {
			for(int k=0; k<c1; ++k) {
				mult[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	
	return mult;
}

int main()
{
    // dim2 A = {{1,2,-1},
    //           {2,0,1}};
    // dim2 B = {{3,1},
    //           {0,-1},
    //           {-2,3}
    //          };
    dim2 A={{1,2,5},{3,4,4},{1,1,1}};
    dim2 B = {{0,2,7},{5,4,4},{-8,8,0}};
    auto C = multiplyMatrices(A, B);
    for(int i=0; i<3; i++) {
        for (int j=0; j<3; j++){ 
            cout << C[i][j] << "\t";
        }
        cout << endl;
    }
    return 0;
    
    
}
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<random>
#include<vector>
using namespace std;
const int n =10;
int main()
{
    double r[n];
    for (int i=0; i<n; i++)
    {
        r[i] = ((double) rand() / (RAND_MAX));
        cout << r[i]  << " ";
    }
    cout << endl;
}

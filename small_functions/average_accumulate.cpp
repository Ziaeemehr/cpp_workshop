#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main()
{
    vector<double> R(10);
    for (int i=0; i<R.size(); i++)
        R[i] = i;

    double R_ave = accumulate(R.begin()+5, R.end(), 0.0)/(R.size()-5);

    cout << R_ave << endl;

    return 0;
}


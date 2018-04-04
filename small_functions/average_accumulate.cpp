#include <iostream>
#include <vector>
#include <numeric>

using namespace std;



inline double mean1(const vector<double>& R, const int id)
{
	/*average the vector from element "id" to end of the vector */
	return accumulate(R.begin()+id, R.end(), 0.0)/(R.size()-id);
}

double mean2(const vector<double>& x, const int indx_cut)
{
    double result = 0;
    int n = x.size() - indx_cut;

    for (int i=indx_cut; i<x.size(); i++)
        result += x[i];
    
    return result/(double)n;
}


int main()
{

	// initialize a vector
    vector<double> R(20000);
    for (int i=0; i<R.size(); i++)
        R[i] = i;


    double R_ave1 = mean1(R, 5);
    double R_ave2 = mean2(R, 5);

    cout << R_ave1 << endl;
    cout << R_ave2 << endl;

    return 0;
}


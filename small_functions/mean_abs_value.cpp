#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

inline void abs_value(vector<double>& numbers)
{
    for (unsigned int i = 0; i < numbers.size(); i++)
    {
        if (numbers[i] < 0)
            numbers[i] *= -1; //make positive.    _OR_   use numbers[i] = abs(numbers[i]);
        std::cout << numbers[i] << std::endl;
    }

}

inline double mean(const vector<double> &v, const int id)
{
    /*average the vector from element "id" to end of the vector */
    return accumulate(v.begin() + id, v.end(), 0.0) / (v.size() - id);
}


int main()
{
    vector<vector<double>> v {{1,0, 1.2, -2.3, 10, -10.0},{-2.5, 1.2}};
    abs_value(v[0]);
    abs_value(v[1]);
    cout << mean(v[0],0)<< endl;
    cout << mean(v[1],0)<< endl;

    return 0;
}
/* @PyCpp_channel
 * a simple program to generate sequence of increasing numbers
 * using std::generator for integer values or
 * using arange equivaliet function in numpy python
 * usage :
 * arange(start_value, end_value, step_value)
*/


#include <algorithm>
#include <iostream>
#include <vector>

// use these to avoid writnig std:: every time
// I am not going to use (std namespace std);
using std::vector;
using std::cout;
using std::endl;


struct Generator {
    Generator() : m_value( 0 ) { }
    int operator()() { return m_value++; }
    int m_value;
};


template<typename T>
std::vector<T> arange(T start, T stop, T step = 1) {
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}

int main()
{
    // method 1 using generator (only for integer values):
    std::vector<int> ivec( 10 );
    std::generate( ivec.begin(), ivec.end(), Generator() );

    for (auto i:ivec)
        cout << i << " ";
    cout << endl;
    
    // method 2 using arange like numpy arange function for int or double values:
    // use like any of these
    vector<double> v = arange(5., 10.1, 0.5);  // numpy equivaliet is np.arange(start, end, step)
    auto v1 = arange<int>(0, 10); 
    
    for (auto i:v)
        std::cout << i << " ";
    cout << endl;
    
    for (auto i:v1)
        std::cout << i << " ";
    cout << endl;
    
    return 0;
}


// output :
// 0 1 2 3 4 5 6 7 8 9 
// 5 5.5 6 6.5 7 7.5 8 8.5 9 9.5 10 
// 0 1 2 3 4 5 6 7 8 9 

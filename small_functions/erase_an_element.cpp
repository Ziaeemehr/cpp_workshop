#include<iostream>
#include<vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
int main()
{
    int N{10};
    vector<int> vec(N);
    
    for (int i=0; i<N; i++)
        vec[i] = i;
    //C++ Erase vector element by value rather than by position
    vec.erase(std::remove(vec.begin(), vec.end(), 8), vec.end());
    for (auto v : vec)
        cout<<v<<"\t";
    cout<<endl;
    
    return 0;
}

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int i = -1;
    vector<vector<int> >N;
    vector<int>a(3,1);
    vector<int>b(4,3);
    

    N.push_back(a);
    N.push_back(b);
    
    for (int i=0; i<N.size(); i++) {
        for (int j=0; j<N[i].size(); j++)
            cout << N[i][j] << " ";
        cout << "\n";
    }
    
    
    return 0;
}
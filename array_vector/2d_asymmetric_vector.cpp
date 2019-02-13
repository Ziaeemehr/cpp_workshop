// resizing vector
#include <iostream>
#include <vector>

using std::cout;
using std::vector;

int main ()
{
    std::vector<vector<int> > myvector;

    myvector.resize(2);
    for (int i=0; i<10; i++)
        myvector[0].push_back(i);
    for (int i=0; i<6; i++)
        myvector[1].push_back(i);

    for (int i=0; i<myvector.size(); i++) {
        for(int j=0; j<myvector[i].size(); j++) {
            cout<<myvector[i][j]<< "\t";
        }
        cout << "\n";
    }
  return 0;
}

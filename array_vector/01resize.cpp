// resizing vector
#include <iostream>
#include <vector>

using std::cout;
using std::vector;

int main ()
{
  std::vector<int> myvector;

  // set some initial content:
  for (int i=1;i<10;i++) myvector.push_back(i);
 
  myvector.resize(5);
  myvector.resize(8,100);
  myvector.resize(12);

  
  for (auto i : myvector)
    cout << ' ' << i;
  cout << '\n'<<'\n';
  
  int n = 2; 
  int m = 5;
  vector<vector<int>> M(n,vector<int>(m,1));
  for (int i=0; i<M.size(); i++){
      for(int j=0; j<M[0].size(); j++){
          cout << M[i][j]<<" ";
      }
      cout<< "\n";
    }
    cout << '\n';
    
    // resizing a 2D vector
    n = 3;
    m = 10;
    M.resize(n);
    for (int i = 0; i < n; ++i)
        M[i].resize(m);
    cout << "resized 2d vector:" << '\n';
    cout << M.size() << ' ' << M[0].size()<<'\n';
    
    for (int i=0; i<M.size(); i++){
      for(int j=0; j<M[0].size(); j++){
          cout << M[i][j]<<" ";
      }
      cout<< "\n";
    }
  
  return 0;
}

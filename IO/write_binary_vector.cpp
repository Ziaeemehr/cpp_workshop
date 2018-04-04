/* 
write array of int  in a loop
*/

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main()
{
  int row = 20;
  int col = 15;
  vector<int> f(col);
  int s=0;
  unsigned fsize=f.size();
  ofstream out("numbers.bin", ios::out | ios::binary);
  if(!out) 
  {
    cout << "Cannot open file.";
    return 1;
  }
  for (int i=0; i<row; i++) 
  {
    for (int j=0; j<col; j++) 
    {
      f[j] = i;
    } 
    // out.write(reinterpret_cast<char *>(&fsize), sizeof(unsigned));
    out.write(reinterpret_cast<char *>(&f[0]), f.size()*sizeof(int));
  }

  out.close();

  return 0;
}
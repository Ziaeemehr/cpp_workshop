/* 
write array of int  in a loop
*/

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
  int row = 20;
  int col = 15;
  int f[col];
  int s=0;

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
      f[j] = s;
    } 
    out.write((char *) &f[0], sizeof f);
    if ((i%2)==0)
      s=1;
    else
      s=0;
  }

  out.close();

  return 0;
}
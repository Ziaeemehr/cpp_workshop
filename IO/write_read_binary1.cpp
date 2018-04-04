/* 
write double in a loop and read array of doubles from binary file
*/

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
  double fnum;
  int i;
  int N=10;

  ofstream out("numbers.bin", ios::out | ios::binary);
  if(!out) {
    cout << "Cannot open file.";
    return 1;
   }
  for (int i=0; i<N; i++)
  {
    fnum = i*0.2;
    out.write((char *) &fnum, sizeof fnum);
  }

  out.close();

  double f[N];
  ifstream in("numbers.bin", ios::in | ios::binary);
  in.read((char *) &f, sizeof f);

  // see how many bytes have been read
  cout << in.gcount() << " bytes read\n";

  for(i=0; i<N; i++) // show values read from file
  cout << f[i] << " ";
  cout << "\n";
  in.close();

  return 0;
}
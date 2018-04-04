/* 
write array of double in a loop
*/

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
  double fnum[3];
  int i;
  int N=10;

  ofstream out("numbers.bin", ios::out | ios::binary);
  if(!out) {
    cout << "Cannot open file.";
    return 1;
   }
  for (int i=0; i<N; i++){
    for (int j=0; j<3; j++)
      fnum[j] = i * 0.1;
    out.write((char *) &fnum[0], sizeof fnum);
  }

  out.close();

  return 0;
}
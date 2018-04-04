// Save and read double array in a binary file : 
// ofstream « File Stream « C++ Tutorial


#include <iostream>
#include <fstream>
using namespace std;

int main()
{
  double fnum[4] = {9.5, -3.4, 1.0, 2.1};
  int i;

  ofstream out("numbers.bin", ios::out | ios::binary);
  if(!out) {
    cout << "Cannot open file.";
    return 1;
   }

  out.write((char *) &fnum, sizeof fnum);

  out.close();

  for(i=0; i<4; i++) // clear array
    fnum[i] = 0.0;

  ifstream in("numbers.bin", ios::in | ios::binary);
  in.read((char *) &fnum, sizeof fnum);

  // see how many bytes have been read
  cout << in.gcount() << " bytes read\n";

  for(i=0; i<4; i++) // show values read from file
  cout << fnum[i] << " ";
  cout << "\n";
  in.close();

  return 0;
}
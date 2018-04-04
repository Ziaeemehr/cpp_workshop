/* 
write array of int  in a loop
*/

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// typedef unsigned char BYTE;

std::vector<int> readFile(const char* filename)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(filename, ios::in | ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<int> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    cout << file.gcount() << " bytes read\n";
    return fileData;
}


int main()
{
  int col = 15;
  vector<int> f(col);
  unsigned fsize=f.size();
  ofstream out("numbers.bin", ios::out | ios::binary);
  if(!out) 
  {
    cout << "Cannot open file.";
    return 1;
  }
  for (int j=0; j<col; j++) 
    {
      f[j] = j;
    } 
    // out.write(reinterpret_cast<char *>(&fsize), sizeof(unsigned));
  out.write(reinterpret_cast<char *>(&f[0]), f.size()*sizeof(int));
  out.close();

  auto V = readFile("numbers.bin");
  cout << V.size()<<"\n";
  for (int i=0; i<V.size(); i++)
  {
    cout << V[i]<< " " <<endl;
  }

  // streampos filesize;
  // char * memblock;

  // ifstream file ("numbers.bin", ios::in|ios::binary|ios::ate);
  // if (file.is_open())
  // {
  //   filesize = file.tellg();
  //   file.seekg (0, ios::end);
  //   // read the data
  //   std::vector<double> fileData(filesize);
  //   file.read ((char*)&fileData, filesize);
  //   file.close();

  // }
  // else cout << "Unable to open file";



  // ifstream in("numbers.bin", ios::in | ios::binary);
  // in.read((char *) &fnum, sizeof fnum);

  // // see how many bytes have been read
  // cout << in.gcount() << " bytes read\n";

  // for(i=0; i<4; i++) // show values read from file
  // cout << fnum[i] << " ";
  // cout << "\n";
  // in.close();


  return 0;
}
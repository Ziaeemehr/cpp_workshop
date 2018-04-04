/*
  Example of how to write binary stuff in C++
  
  Illustrates some of the various quirks/annoyances
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main( int argc, char ** argv)
{
  string X("Hi there");
  unsigned Y = 11;
  vector<double> Z(10,1.0);

  ofstream o("binary_out_test.bin",ios_base::binary);

  //To write a string, print out its length, and then the char *
  //However, X.size() is a temp, and you cannot write a temp,
  //so you need a new variable.
  unsigned Xsize = X.size();
  //The next line is how you write most types:  cast a pointer to a type to a char * and give sizeof the type
  o.write ( reinterpret_cast<char *>(&Xsize),sizeof(unsigned) );
  o.write ( X.c_str(), Xsize*sizeof(char) );

  //Write out that vector

  //Take advantages of the fact that &vector<T>[0] is a pointer to type T.
  //gotta write out the size first!!!
  Xsize=Z.size();
  o.write ( reinterpret_cast<char *>(&Xsize),sizeof(unsigned) );
  o.write( reinterpret_cast<char *>(&Z[0]), Z.size()*sizeof(double) );

  o.close();

  //Now, read it all back in
  ifstream in("binary_out_test.bin",ios_base::binary);

  unsigned stringsize;
  in.read( reinterpret_cast<char *>(&stringsize),sizeof(unsigned) );

  //Cannot read to a string directly, must have buffer intermediate
  vector<char> temp(stringsize);
  in.read( reinterpret_cast<char *>(&temp[0]), stringsize*sizeof(char) );
  string thestring(temp.begin(),temp.end());

  unsigned vsize;
  in.read( reinterpret_cast<char *>(&vsize),sizeof(unsigned) );
  vector<double> thedoubles(vsize);
  in.read( reinterpret_cast<char *>(&thedoubles[0]), vsize*sizeof(double) );

  cout << stringsize << ' ' << thestring << '\n'
       << vsize << ' ';
  for(unsigned i = 0 ; i < thedoubles.size() ; ++i )
    {
      cout << thedoubles[i] << ' ';
    }
  cout << '\n';
  
  in.close();
}

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
  double a = 13.7;
  
  ofstream o("binary_out_test.bin",ios_base::binary);
  unsigned asize = sizeof(a);
  o.write(reinterpret_cast<char *>(&a),sizeof(a));
  o.write(reinterpret_cast<char *>(&a),sizeof(a));
  o.close();

  return 0;

  asize = sizeof(a);
  //The next line is how you write most types:  cast a pointer to a type to a char * and give sizeof the type
  o.write( reinterpret_cast<char *>(&asize),sizeof(unsigned) );
  o.write( reinterpret_cast<char *>(&a), sizeof(double) );
  o.close();

  //Now, read it all back in
  ifstream in("binary_out_test.bin",ios_base::binary);

  in.read( reinterpret_cast<char *>(&asize),sizeof(unsigned) );
  double thedouble;
  in.read( reinterpret_cast<char *>(&thedouble), asize*sizeof(double) );

  cout << thedouble << '\n';  
  in.close();
}

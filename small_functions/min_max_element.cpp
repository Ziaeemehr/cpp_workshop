// min_element/max_element example
#include <iostream>     // std::cout
#include <algorithm>    // std::min_element, std::max_element
#include <vector>

bool myfn(int i, int j) { return i<j; }

struct myclass {
  bool operator() (int i,int j) { return i<j; }
} myobj;



int main () {
  int myints[] = {3,7,2,5,6,4,9};

  // using default comparison:
  std::cout << "The smallest element is " << *std::min_element(myints,myints+7) << '\n';
  std::cout << "The largest element is "  << *std::max_element(myints,myints+7) << '\n';

  // using function myfn as comp:
  std::cout << "The smallest element is " << *std::min_element(myints,myints+7,myfn) << '\n';
  std::cout << "The largest element is "  << *std::max_element(myints,myints+7,myfn) << '\n';

  // using object myobj as comp:
  std::cout << "The smallest element is " << *std::min_element(myints,myints+7,myobj) << '\n';
  std::cout << "The largest element is "  << *std::max_element(myints,myints+7,myobj) << '\n';

  std::vector<int> vint {3,7,2,5,6,4,9};
  // using default comparison:
  std::cout << "The smallest element is *** " << *std::min_element(vint.begin(),vint.end()) << '\n';
  std::cout << "The largest element is "  << *std::max_element(vint.begin(),vint.end()) << '\n';

  // Best way in terms of ease of use is to use std::max_element on the vector of vectors
  std::vector<std::vector<int>> v2 {{3,7,2,5,6,4,9},{0,1,6,3,8,13,1}};
  int tmp=-1000;

  std::cout << "The largest element v2[0] "  << *std::max_element(v2[0].begin(),v2[0].end()) << '\n';
  std::cout << "The largest element v2[1] "  << *std::max_element(v2[1].begin(),v2[1].end()) << '\n';

  for (int i=0; i<v2.size(); i++) {
    for (int j=0; j<v2[0].size(); j++) {
      if (v2[i][j] > tmp) 
        tmp = v2[i][j];
    }
  }

  std::cout << "The largest element is " << tmp << "\n";

  return 0;
}
// find example
#include <iostream>     // std::cout
#include <algorithm>    // std::find
#include <vector>       // std::vector
#include <cstdlib>  

using namespace std;

bool IsMatch(double x)
{
  double val = 10.5; 
  double tolerance = 1e-3;
  return abs(x-val) < tolerance;
}

int find_element(const vector<double>& v, const double val, const double tol)
  {
    int i;
    for ( i=0; i < v.size(); i++)
    {
      if (abs(val-v[i]) < tol) 
      {
        cout << i << " " << v[i]<< endl;
        return i; 
      }
    }
      
      cout << "element not found!\n";
      exit(0);
   
  }

// struct dbl_compare 
// {
//     dbl_compare(double v, double d) val(v), delta(d) { }
//     inline bool operator()(const double &x) const {
//         return abs(x-val) < delta;
//     }
// private:
//     double val, delta;
// };

// class dbl_cmp
// {
// public:
//   dbl_cmp(double ival, double itol)
//   {
//     val = ival;
//     tol = itol;
//   }
//   bool compare(double x)
//   {
//     return (abs(x-val) < tol);
//   }
// private:
//   double tol;
//   double val;
// };


int main () {
  // using std::find with array and pointer:
  int myints[] = { 10, 20, 30, 40 };
  int * p;

  p = std::find (myints, myints+4, 30);
  if (p != myints+4)
    std::cout << "Element found in myints: " << *p << '\n';
  else
    std::cout << "Element not found in myints\n";

  // using std::find with vector and iterator:
  std::vector<int> myvector (myints,myints+4);
  std::vector<int>::iterator it;

  it = find (myvector.begin(), myvector.end(), 30);
  if (it != myvector.end())
    std::cout << "Element found in myvector: " << *it << '\n';
  else
    std::cout << "Element not found in myvector\n";

  vector<double> v {1.2, 4.3, 10.5, 5.2};
  double a = 10.5;
  double epsilon=0.01;
  std::vector<double>::iterator it1 = std::find_if(v.begin(),v.end(), IsMatch);
  std::cout << "The matched value is " << *it1 << '\n';
  ptrdiff_t pos = it1 - v.begin();
  if(pos <= v.size()) {
    cout << pos<<endl;
  }
  // dbl_cmp A(10.5,0.01);
  // std::vector<double>::iterator it2 = std::find_if(v.begin(),v.end(), A.compare());
  find_element(v, 11.5, 0.01);

  

  return 0;
}
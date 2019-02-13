#include <iostream>
#include <math.h>
#include <assert.h>

// https : //stackoverflow.com/questions/15634400/continous-angles-in-c-eq-unwrap-function-in-matlab

void unwrap_array(double *in, double *out, int len)
{
  out[0] = in[0];
  for (int i = 1; i < len; i++)
  {
    double d = in[i] - in[i - 1];
    d = d > M_PI ? d - 2 * M_PI : (d < -M_PI ? d + 2 * M_PI : d);
    out[i] = out[i - 1] + d;
  }
}


//-----------------------------------------------------------//
int main()
{
  int n=5;
  double in[] = { 0., 0.78539816, 1.57079633, 5.49778714, 2*M_PI };
  double out[] = {0.0, 0.0, 0.0, 0.0 ,0.0};
  unwrap_array(in, out, n);
  for (int i=0; i<n; i++)
    std::cout << out[i] << " ";
  std::cout<< "\n";
}
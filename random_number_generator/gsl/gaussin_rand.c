/* PJ Adapted from
http://lcgapp.cern.ch/project/cls/workbook/gsl-howto.html
*/

/* Objective: Fundamental demonstration of random generator usage,
   how to draw random variates from normal and uniform 
*/

/* Uses MT19937 as the generator */
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

int main (void)
{
  const gsl_rng_type * T;
  gsl_rng * r;
  int i;  
  double sigma = 10; 


  // create random number generator 
  r = gsl_rng_alloc (gsl_rng_mt19937);

  
  for (i = 0; i < 20; i++) 
    {
      double u = gsl_rng_uniform (r);
      double v = gsl_ran_gaussian(r, sigma); 
      fprintf(stderr, "i=%d: u=%f v=%f \n", i, u, v);
    }

  gsl_rng_free (r);


  return 0;
}

/*
Local Variables:
compile-command: "gcc -o -pg -g program -Wall example-1.c -lgsl -lgslcblas -lm"
End:
*/
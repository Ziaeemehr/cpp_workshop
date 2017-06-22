/* system example : DIR */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <iostream>

int main ()
{
  int i;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  printf ("Executing command DIR...\n");
  i=system ("dir");
  printf ("The value returned was: %d.\n",i);
  std::cout << system("echo 'this is a test'")<<std::endl;
  return 0;
}

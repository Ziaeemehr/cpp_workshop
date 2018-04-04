#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <iostream>
#include <omp.h>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	
	int nthreads = omp_get_num_threads();
	printf("the number of threads  %d\n", nthreads );
	string command1;

	#pragma omp parallel for private(command1)
	for (int i=0; i<8; i++)
	{
		int tid = omp_get_thread_num();
		printf(" thread %d\n", tid);
		system(("echo " + to_string(i)).c_str());
	}
	return 0;
}

// #pragma omp parallel for private(command1)
// 	for (int i=0; i<8; i++)
// 	{
// 		int tid = omp_get_thread_num();
// 		printf(" thread %d\n", tid);
// 		command1 = "echo " + to_string(i);
// 		system(command1.c_str());
// 	}
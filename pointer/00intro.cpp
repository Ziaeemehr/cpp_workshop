#include <stdio.h>
#include<iostream>
#include <iomanip>

using namespace std;
int main () {

   int  var = 20;   /* actual variable declaration */
   int  *ip;        /* pointer variable declaration */

   ip = &var;  /* store address of var in pointer variable*/
   cout << "Address of var variable:" << &var <<endl;

   /* address stored in pointer variable */
    printf("Address stored in ip variable: %p\n", ip );
    cout <<"Address stored in ip variable:" << ip <<endl;

   /* access the value using the pointer */
   printf("Value of *ip variable: %d\n", *ip+1 );

   return 0;
}

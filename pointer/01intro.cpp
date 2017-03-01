#include <stdio.h>
#include<iostream>
#include <iomanip>

using namespace std;



int f1(int *n1, int *n2,double *n3){
    (*n1)++;
    (*n2)--;
    (*n3)--;
    cout << *n1<<"\t"<<*n2<<"\t"<< *n3<<endl;
    return (*n1)+(*n2);
}


int main () {

   int a1;
   int a2;
   a1 = 2;
   a2 = 4;
   double a3 = 22.;
   int r;
   cout <<"befor f1"<<endl;
   cout <<"a1: "<<a1<<" a2: "<<a2<<"   a3: "<< a3<<endl;
   r = f1(&a1,&a2,&a3);
   cout << "after f1   "<< endl;
   cout <<"a1: "<<a1<<" a2: "<<a2<<"   a3: "<< a3<<endl;

   return 0;
}

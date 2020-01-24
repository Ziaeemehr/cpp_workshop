#include <cstdio>
#include <cstdlib>
#include <vector>
#include "spline.h"


using std::vector;

int main(int argc, char** argv) {
   if(argc<1) {
      printf("usage: %s <>\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   std::vector<double> X(5), Y(5);
   vector<vector<double>> x;
   
   X[0]=0.1;
   X[1]=0.4;
   X[2]=1.2;
   X[3]=1.8;
   X[4]=2.0;
   Y[0]=0.1;
   Y[1]=0.7;
   Y[2]=0.6;
   Y[3]=1.1;
   Y[4]=0.9;
   
   x = {X, Y};

   tk::spline s;
   s.set_points(x[0], x[1]);

   FILE* of0=fopen("x0", "w");
   for(size_t i=0; i<X.size(); i++){
      fprintf(of0,"%f %f\n", X[i], Y[i]);
   }
   fprintf(of0, "\n");
   fclose(of0);
   
   FILE* of1=fopen("x1", "w");
   for(int i=-50; i<250; i++){
      double x0=0.01*i;
      fprintf(of1, "%f %f\n", x0, s(x0));
   }
   fprintf(of1, "\n");
   fclose(of1);

   return EXIT_SUCCESS;
}



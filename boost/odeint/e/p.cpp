#include<iostream> 
#include<vector>
#include<fstream>
#include <string>

using namespace std;


const int n = 2, nstep = 10;

void xprsys(int n,double *x,double *f)
{
    f[0] = 0.0;
    f[1] = x[1] + 2.0 * x[2];
    f[2] = 3.0 * x[1] + 2.0 * x[2];
}


void rks4(int n, double h, double *x, int nstep)
{
    double y[n+1];
    double f1[n+1],f2[n+1],f3[n+1],f4[n+1];
    int i,k;
    cout <<"k"<<"\t"<<"t"<<"\t"<<"x"<<"\t"<<"y"<<endl;
    cout << 0.0 << "\t";
    for(int i=0; i<=n; i++){
	cout << x[i]<< "\t";
    }
    cout << endl;
    
    for (k=1; k<=nstep; k++){
	xprsys(n,x,f1);
	for(int it=0; it<=n; it++){
	    y[i] = x[i] + 0.5 * h * f1[it];
	}
	xprsys(n,y,f2);
	for(int it=0; it<=n; it++){
	    y[i] = x[i] + 0.5 * h * f2[it];
	}
	xprsys(n,x,f3);
	for(int it=0; it<=n; it++){
	    y[i] = x[i] + h * f3[it];
	}
	xprsys(n,x,f4);
	for(int it=0; it<=n; it++){
	    x[i] = x[i] + h/6.0 * (f1[it] + 2.0 * (f2[it] + f3[it]) + f4[it]);
	}

	cout << k <<"\t" << x[0]<< "\t"<<x[1]<<"\t"<<x[2]<<endl;
    }
}


int main()
{
    double a = 0.0;
    double b = 0.2;
    double h = (b-a)/nstep;
    cout << nstep<<"\t"<<h<< endl;
    
    double x[n+1];
    x[0] = 0.0, x[1]= 6.0, x[2]= 4.0;
    rks4(n,h,x,nstep);
    return 0;
}

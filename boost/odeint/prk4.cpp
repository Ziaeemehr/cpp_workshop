#include<iostream> 
#include<vector>
#include<fstream>
#include <string>
#include<iomanip>

using namespace std;

const int n = 2, nstep = 10;

void xprsys(int n,vector<double>& x,vector<double>& f)
{
    f[0] = 0.0;
    f[1] = x[1] + 2.0 * x[2];
    f[2] = 3.0 * x[1] + 2.0 * x[2];
}


void rks4(int n, double h, vector<double>& x, int nstep)
{
    vector<double> y(n+1);
    vector<double> f1(n+1),f2(n+1),f3(n+1),f4(n+1);
    int i,k;
    
    cout<< setprecision(10);
    
    cout <<"k"<<"\t"<<"t"<<"\t"<<"x"<<"\t"<<"y"<<endl;
    cout << 0.0 << "\t";
    for(int i=0; i<=n; i++){
	cout << x[i]<< "\t";
    }
    cout << endl;
        
    for (k=1; k<=nstep; k++){
	xprsys(n,x,f1);
	for(int i=0; i<=n; i++){
	    y[i] = x[i] + 0.5 * h * f1[i];
	}
	xprsys(n,y,f2);
	for(int i=0; i<=n; i++){
	    y[i] = x[i] + 0.5 * h * f2[i];
	}
	xprsys(n,y,f3);
	for(int i=0; i<=n; i++){
	    y[i] = x[i] + h * f3[i];
	}
	xprsys(n,y,f4);
	for(int i=0; i<=n; i++){
	    x[i] = x[i] + (h/6.0) * (f1[i] + 2.0 * f2[i] + 2. * f3[i] + f4[i]);	   
	}
	cout << k <<"\t" << h*k<< "\t" <<x[1]<<"\t"<<x[2]<<endl;
    }
}


int main()
{
    vector<double> x(n + 1);
    double a = 0.0;
    double b = 0.2;
    double h = (b-a)/nstep;
    cout << nstep<<"\t"<<h<< endl;    
    x[0] = 0.0, x[1]= 6.0, x[2]= 4.0;
    rks4(n,h,x,nstep);
    
    return 0;
}

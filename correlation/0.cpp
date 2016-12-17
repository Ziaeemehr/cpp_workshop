#include<iostream>
#include<cstdlib>
using namespace std;

public static void GetPearson(double[] x, 
                              double[] y,
                              ref Double r,ref Double prob)
{
    //will regularize the unusual case of complete correlation
    const double TINY=1.0e-20;
    int j,n=x.Length;
    Double yt,xt,t,df;
    Double syy=0.0,sxy=0.0,sxx=0.0,ay=0.0,ax=0.0;
    for (j=0;j&lt;n;j++) {
    //finds the mean
        ax += x[j];
        ay += y[j];
    }
    ax /= n;
    ay /= n;
    for (j=0;j&lt;n;j++) {
     // compute correlation coefficient
        xt=x[j]-ax;
        yt=y[j]-ay;
        sxx += xt*xt;
        syy += yt*yt;
        sxy += xt*yt;
    }
    r=sxy/(Math.Sqrt(sxx*syy)+TINY);
    //for a large n
    prob=erfcc(Math.Abs(z*Math.Sqrt(n-1.0))/1.4142136);
}

public static Double erfcc(Double x)
{
    Double t, z, ans;
    z = Math.Abs(x);
    t = 1.0 / (1.0 + 0.5 * z); 
    ans = t * Math.Exp(-z * z - 1.26551223 + t * (1.00002368 + 
                           t * (0.37409196 + t * (0.09678418 +
                          t * (-0.18628806 + t * (0.27886807 + 
                          t * (-1.13520398 + t * (1.48851587 + 
                     t * (-0.82215223 + t * 0.17087277)))))))));
    return x >= 0.0 ? ans : 2.0 - ans;
}

int main()
{
    double[] x = new double[4]{525, 300, 450, 300, 400, 500, 
                            550, 125, 300, 400, 500, 550};

    double[] y = new double[4]{250, 225, 275, 350, 
                            325, 375, 450, 400, 500, 550, 600, 525};

    double r = 0, prob = 0;
    GetPearson(x, y,ref r,ref prob);
    
}


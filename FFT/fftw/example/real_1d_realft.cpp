#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <math.h>
//============================================================
#define REAL 0
#define IMAG 1
#define NUM_POINTS (1024 * 4)
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
//============================================================
void realft(double data[], unsigned long n, int isign);
void acquire_signal(double *signal, int fs);
void realft(double data[], unsigned long n, int isign);
void four1(double data[], unsigned long nn, int isign);
//============================================================



int main()
{
	int fs = 1024; // Sample rate in Hz
	unsigned flags{0};
	double *signal = new double[NUM_POINTS];

	acquire_signal(signal, fs);
	realft(signal, NUM_POINTS, 1);

	//save signal and result
	std::ofstream f1;
	f1.open("result.txt");
	double INV_NUM_POINTS = 1.0 / NUM_POINTS;
	for (int i = 0; i < (NUM_POINTS / 2 + 1); i++)
	{
		f1 << 0.5*(double)i * fs * INV_NUM_POINTS << " " << signal[i] * INV_NUM_POINTS * 2.0 << "\n";
	}
	f1.close();
	delete[] signal;

	return 0;
}




//============================================================
void acquire_signal(double *signal, int fs)
{
	/* Generate two sine waves of different frequencies and
     * amplitudes.
     */

	int i;
	for (i = 0; i < NUM_POINTS; ++i)
	{
		double theta = (double)i / (double)fs;
		signal[i] = 1.0 * sin(50.25 * 2.0 * M_PI * theta) +
					0.5 * sin(80.50 * 2.0 * M_PI * theta);
	}
}
//============================================================
void realft(double data[], unsigned long n, int isign)
{
  //void four1(double data[], unsigned long nn, int isign);
	unsigned long i,i1,i2,i3,i4,np3;
	double c1=0.5,c2,h1r,h1i,h2r,h2i;
	double wr,wi,wpr,wpi,wtemp,theta;

	theta=3.141592653589793/(double) (n>>1);
	if (isign == 1) {
		c2 = -0.5;
		four1(data,n>>1,1);
	} else {
		c2=0.5;
		theta = -theta;
	}
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	np3=n+3;
	for (i=2;i<=(n>>2);i++) {
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
		h1r=c1*(data[i1]+data[i3]);
		h1i=c1*(data[i2]-data[i4]);
		h2r = -c2*(data[i2]+data[i4]);
		h2i=c2*(data[i1]-data[i3]);
		data[i1]=h1r+wr*h2r-wi*h2i;
		data[i2]=h1i+wr*h2i+wi*h2r;
		data[i3]=h1r-wr*h2r+wi*h2i;
		data[i4] = -h1i+wr*h2i+wi*h2r;
		wr=(wtemp=wr)*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
	}
	if (isign == 1) {
		data[1] = (h1r=data[1])+data[2];
		data[2] = h1r-data[2];
	} else {
		data[1]=c1*((h1r=data[1])+data[2]);
		data[2]=c1*(h1r-data[2]);
		four1(data,n>>1,-1);
	}
}
//============================================================
/* (C) Copr. 1986-92 Numerical Recipes Software V,3. */
void four1(double data[], unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	double tempr,tempi;

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}
// #undef SWAP
/* (C) Copr. 1986-92 Numerical Recipes Software V,3. */
// ** END OF THE MAIN PROGRAM **
#ifndef __FOURIER_H__
#define __FOURIER_H__

#include <Complex.h>

#define PI			3.1415926535897932f
#define SWAP(a,b)	tempr=(a); (a)=(b); (b)=tempr
#define MIN(a,b)	(a>b?a:b)

class CFourier
{
protected:
	void FFT_1D(Complex* &, int, int);
	void FFT_2D(Complex** &, int, int, int);
	void four1(float* &, unsigned long, int);
public:
	void FFT_Original(Complex** &, int, int);
	void FFT_Inverse(Complex** &, int, int);
};

#endif	//__FOURIER_H__

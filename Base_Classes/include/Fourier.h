#ifndef __FOURIER_H__
#define __FOURIER_H__

#include <Complex.h>
#include "Constants.h"

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

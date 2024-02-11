#include <math.h>
#include "Fourier.h"

void CFourier::FFT_Original(Complex** &data, int xsize, int ysize)
{
	FFT_2D(data, xsize, ysize, -1);
}

void CFourier::FFT_Inverse(Complex** &data, int xsize, int ysize)
{
	FFT_2D(data, xsize, ysize, 1);
}

void CFourier::FFT_1D(Complex* &data, int xsize, int type)
{
	float *temp_data;
  
	temp_data = new float[2*xsize];

	for (int i=0; i<2*xsize; i+=2) 
	{
		temp_data[i]   = data[i/2].getReal();
		temp_data[i+1] = data[i/2].getImaginary();
	}
  
	four1(temp_data,xsize,type);

	for (int i=0; i<2*xsize; i+=2) 
	{
		data[i/2] = Complex(temp_data[i], temp_data[i+1]);
	}

	delete[] temp_data;
}

void CFourier::FFT_2D(Complex** &data, int xsize, int ysize, int type)
{
	Complex *temp_data;
	float Energy1, Energy2;
	int maxsize;
  
	maxsize = (xsize>ysize)?xsize:ysize;
  
	temp_data = new Complex[maxsize];

	// Computing Energy Of Block
	Energy1 = 0.0;
	for (int i=0;i<xsize;i++) 
	{
		for (int j=0;j<ysize;j++) 
		{
			Energy1 += (data[i][j].getReal() * data[i][j].getReal() + data[i][j].getImaginary() * data[i][j].getImaginary());
		}
	}
  
	// 1D_FFT in lines
	for (int i=0; i<xsize; i++) 
	{
		for (int j=0; j<ysize; j++) 
		{
			temp_data[j] = data[i][j];
		}
		FFT_1D(temp_data,ysize,type);
		for (int j=0; j<ysize; j++) 
		{
			data[i][j] = temp_data[j];
		}
	}
  
	// 1D_FFT in columns
	for (int j=0; j<ysize; j++) 
	{
		for (int i=0; i<xsize; i++) 
		{
			temp_data[i] = data[i][j];
		}
		FFT_1D(temp_data,xsize,type);
		for (int i=0; i<xsize; i++) 
		{
			data[i][j] = temp_data[i];
		}
	}

	// Computing Energy Of Transformed Block
	Energy2 = 0.0;
	for (int i=0;i<xsize;i++) 
	{
		for (int j=0;j<ysize;j++) 
		{
			Energy2 += (data[i][j].getReal() * data[i][j].getReal() + data[i][j].getImaginary() * data[i][j].getImaginary());
		}
	}

	// Adjusting Coeffidents for having same amount of Energy
	for (int i=0;i<xsize;i++) 
	{
		for (int j=0;j<ysize;j++) 
		{
			data[i][j] = data[i][j] * Complex(sqrt(Energy1/Energy2), 0.0);
		}
	}
    
	delete[] temp_data;
}

/* Replaces data[1..2*size] by its discrete Fourier transform, if isign
 * input as -1; or replaces data[1..2*size] by size times its inverse
 * Fourier transform, if isign is input as 1. data is a complex array
 * of length size, or equivalently, a real array of length 2*size. size MUST
 * be an integer power of 2 (this is not checked for!).
 */
void CFourier::four1(float* &data, unsigned long size, int isign)
{
	unsigned long n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	float tempr,tempi;
	           
	n = size << 1;
	j = 1;

	for (i=1; i<n; i+=2) 
	{
		if (j > i) 
		{
			SWAP(data[j-1], data[i-1]);
			SWAP(data[j], data[i]);
		}
		m = n >> 1;
		
		while (m >= 2 && j > m) 
		{
			j -= m;
			m >>= 1;
		}
		j += m;
	}
  
	/* Here begins the Danielson-Lanczos section of the routine. */
	mmax = 2;

	while (n > mmax) 
	{
		istep = mmax << 1;
		theta = isign * (2 * PI / mmax);
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;

		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		
		for (m=1; m<mmax; m+=2) 
		{
			for (i=m; i<=n; i+=istep) 
			{
				j = i + mmax;
				tempr = wr * data[j-1] - wi * data[j];
				tempi = wr * data[j] + wi * data[j-1];
				data[j-1] = data[i-1] - tempr;
				data[j] = data[i] - tempi;
				data[i-1] += tempr;
				data[i] += tempi;
			}
			wr = (wtemp = wr) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax=istep;
	}
}

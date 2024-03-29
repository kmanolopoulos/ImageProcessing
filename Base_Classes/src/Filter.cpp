#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "Filter.h"

using namespace std;

//                        //
//                        //
// Filter class routines  //
//                        //
//                        //

Filter::Filter(FilterType type, int size)
{
	Allocate(size);
	Initialize(type);
}

Filter::~Filter()
{
	DeAllocate();
}

void Filter::Allocate(int size) 
{
	if (size<0)
	{
		cout << "Bad Allocation size (" << size << ")\n";
		return;
	}

	length = size;

	if ((value = new float*[length]) == NULL)
	{
		cout << "Out of memory\n";
		exit(1);
	}

	for (int i=0;i<length;i++) 
	{
		if ((value[i] = new float[length]) == NULL) 
		{
			cout << "Out of memory\n";
			exit(1);
		}
	}
}

void Filter::DeAllocate()
{
	if (length < 0) return;

	for (int i=0; i<length; i++) 
	{
		delete[] value[i];
	}
	delete[] value;

	length = -1;
}

void Filter::Initialize(FilterType type)
{
	int x, y;

	for (int i=0; i<length;i++)
	{
		x = i - length / 2;

		for (int j=0; j<length; j++)
		{
			y = j - length / 2;

			switch (type)
			{
				case IDENTITY:
					value[i][j] = FunctionIdentity(x, y);
					break;
				case GAUSSIAN:
					value[i][j] = FunctionGauss(x, y, 1.0);
					break;
				case LAPLACIAN:
					value[i][j] = FunctionLaplace(x, y, 1.4);
					break;
				case PREWITT_X:
					value[i][j] = FunctionPrewittX(x, y);
					break;
				case PREWITT_Y:
					value[i][j] = FunctionPrewittY(x, y);
					break;
				case SOBEL_X:
					value[i][j] = FunctionSobelX(x, y);
					break;
				case SOBEL_Y:
					value[i][j] = FunctionSobelY(x, y);
					break;
				case BOX_LINEAR:
					value[i][j] = FunctionBoxLinear(x, y);
					break;
				case SHARPEN:
					value[i][j] = FunctionSharpen(x, y);
					break;
				default:
					value[i][j] = FunctionIdentity(x, y);
					break;
			}
		}
	}
}

void Filter::GetSize(int &size)
{
	size = length;
}

float Filter::GetValue(int x, int y)
{
	if ((x<0) || (y<0) || (x>=length) || (y>=length)) 
	{
		cout << "Bad value in getValue: (" << x << "," << y << ")\n";
		return 0.0;
	}

	return value[x][y];
}

float Filter::FunctionIdentity(int x, int y)
{
	if ((x == 0) && (y == 0))
	{
		return 1.0;
	}
	
	return 0.0;
}

float Filter::FunctionGauss(int x, int y, float sigma)
{
	double dx = (double)x;
	double dy = (double)y;
	double dsigma = (double)sigma;
	return (1.0 / (2.0 * PI * sigma * sigma)) * (float)exp(-((dx * dx + dy * dy) / (2.0 * dsigma * dsigma)));
}

float Filter::FunctionLaplace(int x, int y, float sigma)
{
	double dx = (double)x;
	double dy = (double)y;
	double dsigma = (double)sigma;
	return - (1.0 / (PI * sigma * sigma * sigma * sigma)) * (1.0 - ((dx * dx + dy * dy) / (2.0 * sigma * sigma))) * (float)exp(-((dx * dx + dy * dy) / (2.0 * dsigma * dsigma)));
}

float Filter::FunctionPrewittX(int x, int y)
{
	return (float)x;
}

float Filter::FunctionPrewittY(int x, int y)
{
	return (float)y;
}

float Filter::FunctionSobelX(int x, int y)
{
	float fx = (float)x;
	float fy = (float)y;
	
	if ((fx == 0) && (fy == 0))
	{
		return 0.0;
	}

	return (fx / ( fx * fx + fy * fy));
}

float Filter::FunctionSobelY(int x, int y)
{
	float fx = (float)x;
	float fy = (float)y;
	
	if ((fx == 0) && (fy == 0))
	{
		return 0.0;
	}
	
	return (fy / ( fx * fx + fy * fy));
}

float Filter::FunctionBoxLinear(int x, int y)
{
	float flength = (float)length;
	
	return (1.0 / (flength * flength));
}

float Filter::FunctionSharpen(int x, int y)
{
	float flength = (float)length;

	if ((x == 0) && (y == 0))
	{
		return 2.0 - (1.0 / (flength * flength));
	}
	
	return (-1.0 / (flength * flength));
}

void Filter::ShowMatrix()
{
	for (int i=0; i<length; i++)
	{
		cout << "[";
		for (int j=0; j<length; j++)
		{
			cout << value[i][j];
			
			if (j < length - 1)
			{
				cout << ",";
			}
		}
		cout << "]\n";
	}
}

#include <iostream>
#include <fstream>
#include <string.h>
#include "Filter.h"

using namespace std;

//                        //
//                        //
// Filter class routines  //
//                        //
//                        //

Filter::Filter(FilterType type)
{
	Allocate(5);
	Select(type);
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

void Filter::Select(FilterType type)
{
	float filter_identity[5][5] =		{
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0, 0.0, 1.0, 0.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 } 
										};
	float filter_edge_laplacian1[5][5] ={
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0, 0.0,-1.0, 0.0, 0.0 },
											{ 0.0,-1.0, 4.0,-1.0, 0.0 },
											{ 0.0, 0.0,-1.0, 0.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 } 
										};
	float filter_edge_laplacian2[5][5] ={
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0,-1.0,-1.0,-1.0, 0.0 },
											{ 0.0,-1.0, 8.0,-1.0, 0.0 },
											{ 0.0,-1.0,-1.0,-1.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 } 
										};
	float filter_edge_prewitt[5][5] =	{
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0,-2.0,-1.0, 0.0, 0.0 },
											{ 0.0,-1.0, 0.0, 1.0, 0.0 },
											{ 0.0, 0.0, 1.0, 2.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 } 
										};
	float filter_edge_sobel[5][5] =		{
											{ 0.0, 0.0, 0.0, 0.0, 0.0 },
											{ 0.0,-2.0,-2.0, 0.0, 0.0 },
											{ 0.0,-2.0, 0.0, 2.0, 0.0 },
											{ 0.0, 0.0, 2.0, 2.0, 0.0 },
											{ 0.0, 0.0, 0.0, 0.0, 0.0 } 
										};
	float filter_gaussian_1[5][5] =		{
											{ 0.0, 0.0,    0.0,   0.0,    0.0 },
											{ 0.0, 0.0625, 0.125, 0.0625, 0.0 },
											{ 0.0, 0.125,  0.25,  0.125,  0.0 },
											{ 0.0, 0.0625, 0.125, 0.0625, 0.0 },
											{ 0.0, 0.0,    0.0,   0.0,    0.0 } 
										};
	float filter_gaussian_2[5][5] =		{
											{ 0.0057, 0.0231, 0.0405, 0.0231, 0.0057 },
											{ 0.0231, 0.9249, 0.1503, 0.9249, 0.0231 },
											{ 0.0405, 0.1503, 0.2370, 0.1503, 0.0405 },
											{ 0.0231, 0.9249, 0.1503, 0.9249, 0.0231 },
											{ 0.0057, 0.0231, 0.0405, 0.0231, 0.0057 } 
										};
	float filter_sharpen[5][5] =		{
											{ 0.0,  0.0,     0.0,      0.0,    0.0 },
											{ 0.0, -0.1111, -0.1111,  -0.1111, 0.0 },
											{ 0.0, -0.1111,  1.8889,  -0.1111, 0.0 },
											{ 0.0, -0.1111, -0.1111,  -0.1111, 0.0 },
											{ 0.0,  0.0,     0.0,      0.0,    0.0 } 
										};

	//
	for (int i=0; i<length; i++)
	{
		for (int j=0; j<length; j++)
		{
			switch (type)
			{
				case IDENTITY:
					value[i][j] = filter_identity[i][j];
					break;
				case LAPLACIAN_1:
					value[i][j] = filter_edge_laplacian1[i][j];
					break;
				case LAPLACIAN_2:
					value[i][j] = filter_edge_laplacian2[i][j];
					break;
				case PREWITT:
					value[i][j] = filter_edge_prewitt[i][j];
					break;
				case SOBEL:
					value[i][j] = filter_edge_sobel[i][j];
					break;
				case GAUSSIAN_1:
					value[i][j] = filter_gaussian_1[i][j];
					break;
				case GAUSSIAN_2:
					value[i][j] = filter_gaussian_2[i][j];
					break;
				case SHARPEN:
					value[i][j] = filter_sharpen[i][j];				
					break;
				default:
					value[i][j] = 0;
					break;
			}
		}
	}
	//
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

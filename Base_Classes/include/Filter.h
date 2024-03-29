#ifndef __FILTER_H__
#define __FILTER_H__

#include "Constants.h"

enum FilterType
{
	IDENTITY, GAUSSIAN, LAPLACIAN, PREWITT_X, PREWITT_Y,  SOBEL_X, SOBEL_Y, BOX_LINEAR, SHARPEN
};

//
// Filter definition
//
class Filter
{
protected:
	int length;
	float **value;
	void Allocate(int);
	void DeAllocate();
	void Initialize(FilterType);
	float FunctionIdentity(int, int);
	float FunctionGauss(int, int, float);
	float FunctionLaplace(int, int, float);
	float FunctionPrewittX(int, int);
	float FunctionPrewittY(int, int);
	float FunctionSobelX(int, int);
	float FunctionSobelY(int, int);
	float FunctionBoxLinear(int, int);
	float FunctionSharpen(int, int);
public:
	Filter(FilterType, int);
	~Filter();
	void GetSize(int &);
	float GetValue(int, int);
	void ShowMatrix();
};

#endif	//__FILTER_H__

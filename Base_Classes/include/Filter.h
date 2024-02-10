#ifndef __FILTER_H__
#define __FILTER_H__

enum FilterType
{
	IDENTITY, LAPLACIAN_1, LAPLACIAN_2, PREWITT,  SOBEL, GAUSSIAN_1, GAUSSIAN_2, SHARPEN
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
	void Select(FilterType);
public:
	Filter(FilterType);
	~Filter();
	void GetSize(int &);
	float GetValue(int, int);
};

#endif	//__FILTER_H__

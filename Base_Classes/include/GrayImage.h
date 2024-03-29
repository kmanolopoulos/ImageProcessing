#ifndef __GRAYIMAGE_H__
#define __GRAYIMAGE_H__

#include "Image.h"
#include "Fourier.h"
#include "GrayPixel.h"
#include "Filter.h"

//
// Image definition
//
class GrayImage:public Image
{
protected:
	int lines,columns;                     // initially (-1,-1)
	GrayPixel **pixel;
	void Allocate(int,int);
	void DeAllocate();
	void Normalize();
public:
	// Constructors
	GrayImage();
	~GrayImage();
	// Basic operations
	virtual void getSize(int &, int &);
	virtual void createNew(int, int);
	GrayPixel getPixel(int, int);
	void setPixel(int, int, GrayPixel);
	// Input/output operations
	virtual void readBmp(const char *);
	virtual void writeBmp(const char *);
	void writeSpectogram(const char *);
	// Image processing operations
	void applyFilter(Filter &);
	void applyLowpassFilter(float);
	void applyBandpassFilter(float, float, float);
	void applySpectogramShift(float, float);
};

#endif	//__GRAYIMAGE_H__

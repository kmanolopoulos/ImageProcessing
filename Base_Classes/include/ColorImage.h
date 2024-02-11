#ifndef __COLORIMAGE_H__
#define __COLORIMAGE_H__

#include "Image.h"
#include "Fourier.h"
#include "ColorPixel.h"
#include "Filter.h"

//
// Image definition
//
class ColorImage:public Image
{
protected:
	int lines,columns;                     // initially (-1,-1)
	ColorPixel **pixel;
	void Allocate(int,int);
	void DeAllocate();
	void Normalize();
public:
	// Constructors
	ColorImage();
	~ColorImage();
	// Basic operations
	virtual void getSize(int &, int &);
	virtual void createNew(int, int);
	ColorPixel getPixel(int,int);
	void setPixel(int,int,ColorPixel);
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

#endif	//__COLORIMAGE_H__

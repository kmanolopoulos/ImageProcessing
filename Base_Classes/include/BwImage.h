#ifndef __BWIMAGE_H__
#define __BWIMAGE_H__

#include "Image.h"
#include "BwPixel.h"

//
// Image definition
//
class BwImage:public Image
{
protected:
	int lines,columns;                     // initially (-1,-1)
	BwPixel **pixel;
	void Allocate(int,int);
	void DeAllocate();
public:
	// Constructors
	BwImage();
	~BwImage();
	// Basic operations
	virtual void getSize(int &, int &);
	virtual void createNew(int, int);
	BwPixel getPixel(int, int);
	void setPixel(int, int, BwPixel);
	// Input/output operations
	virtual void readBmp(const char *);
	virtual void writeBmp(const char *);
	// Image processing operations
	void Denoise();
};

#endif	//__BWIMAGE_H__

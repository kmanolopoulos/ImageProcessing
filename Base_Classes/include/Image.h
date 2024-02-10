#ifndef __IMAGE_H__
#define __IMAGE_H__

//
// Image definition
//
class Image 
{
protected:
	virtual void Allocate(int,int)=0;
	virtual void DeAllocate()=0;
public:
	virtual void getSize(int &,int &)=0;
	virtual void createNew(int,int)=0;
	virtual void readBmp(const char *)=0;
	virtual void writeBmp(const char *)=0;
};

#endif	//__IMAGE_H__

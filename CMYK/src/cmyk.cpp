#include <iostream>
#include <stdlib.h>
#include "LibInclude.h"

using namespace std;

float minimum(float a, float b, float c)
{
	float val;

	if (a<b)   val = a;
	else       val = b;

	if (c<val) val = c;

	return val;
}

int main(int argc, char *argv[])
{
	int lines,columns;
	ColorPixel pixel = { 0.0, 0.0, 0.0 };
	float R,G,B,K;
	ColorImage im;
	ColorImage im1,im2,im3,im4;
  
	if (argc != 2) 
	{
		cout << "Usage: " << argv[0] << " <bmp_file>\n";
		return 1;
	}
  
	im.readBmp(argv[1]);
	im.getSize(lines,columns);
	im1.createNew(lines,columns);
	im2.createNew(lines,columns);
	im3.createNew(lines,columns);
	im4.createNew(lines,columns);
  
	for (int i=0;i<lines;i++) 
	{
		for (int j=0;j<columns;j++) 
		{
			pixel = im.getPixel(i, j);
			R = pixel.RComponent;
			G = pixel.GComponent;
			B = pixel.BComponent;
			K = minimum(255.0-R, 255.0-G, 255.0-B);
			
			pixel.RComponent = 255.0-B-K;
			pixel.GComponent = 255.0-B-K;
			pixel.BComponent = 0.0;
			im1.setPixel(i, j, pixel);

			pixel.RComponent = 255.0-G-K;
			pixel.GComponent = 0.0;
			pixel.BComponent = 255.0-G-K;
			im2.setPixel(i, j , pixel);

			pixel.RComponent = 0.0;
			pixel.GComponent = 255.0-R-K;
			pixel.BComponent = 255.0-R-K;
			im3.setPixel(i, j, pixel);

			pixel.RComponent = K;
			pixel.GComponent = K;
			pixel.BComponent = K;
			im4.setPixel(i, j, pixel);
		}
	}
  
	im1.writeBmp(".\\Coutput.bmp");
	im2.writeBmp(".\\Moutput.bmp");
	im3.writeBmp(".\\Youtput.bmp");
	im4.writeBmp(".\\Koutput.bmp");
        
	return 0;
}

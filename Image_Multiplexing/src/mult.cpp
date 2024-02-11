#include <iostream>
#include <math.h>
#include "LibInclude.h"

using namespace std;

int main(int argc,char **argv)
{
	ColorImage *im;
	ColorImage im_output;
	int N, Nx;
	float ix, iy;
	int lines, columns;
	float Rvalue, Gvalue, Bvalue;
	int templ, tempc;
  
	if (argc < 2) 
	{
		cout << "Usage: " << argv[0] << " <image_list>\n";
		return 0;
	}
	
	N = argc - 1;
	Nx = 1; while ((float)Nx < sqrt(N)) Nx++;
  
	im = new ColorImage[N];

	for (int i=0; i<N; i++)
	{
		ix = 1.0 + (float)(i / Nx);
		iy = 1.0 + (float)(i % Nx);
		im[i].readBmp(argv[i+1]);
		im[i].applySpectogramShift((2.0 * ix - 1.0) * PI / (float)(2 * Nx), (2.0 * iy - 1.0) * PI / (float)(2 * Nx));
		im[i].applyBandpassFilter((2.0 * ix - 1.0) * PI / (float)(2 * Nx), (2.0 * iy - 1.0) * PI / (float)(2 * Nx), PI / (float)(2 * Nx));
	}
  
	im[0].getSize(lines, columns);
	for (int i=1; i<N; i++) 
	{
		im[i].getSize(templ, tempc);
		if ((templ != lines) || (tempc != columns)) 
		{
			cout << "FATAL: images doesn't have the same size as needed!\n";
			delete[] im;
			return 1;
		}
	}
  
	im_output.createNew(lines, columns);
	cout << "Computing Multiplexed Image ... ";
	cout.flush();
	for (int i=0; i<lines; i++) 
	{
		for (int j=0; j<columns; j++) 
		{
			Rvalue = Gvalue = Bvalue = 0.0;
			for (int k=0; k<N; k++) 
			{
				Rvalue += im[k].getPixel(i, j).RComponent;
				Gvalue += im[k].getPixel(i, j).GComponent;
				Bvalue += im[k].getPixel(i, j).BComponent;
			}
			ColorPixel outputPixel = { Rvalue, Gvalue, Bvalue };
			im_output.setPixel(i, j, outputPixel);
		}
	}
	cout << "Done\n";
  
	im_output.writeSpectogram("spec.bmp");
	im_output.writeBmp("mult.bmp");

	delete[] im;

	return 0;
}

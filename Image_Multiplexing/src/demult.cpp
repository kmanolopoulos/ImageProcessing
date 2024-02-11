#include <iostream>
#include <math.h>
#include "LibInclude.h"

using namespace std;

int main(int argc,char **argv)
{
	ColorImage im_output;
	int N, Nx;
	float ix, iy;
	char filename[256];

	if (argc != 3) 
	{
		cout << "Usage: " << argv[0] << " <multiplexed_image> <images_num>\n";
		return 0;
	}
	
	N = atoi(argv[2]);
	Nx = 1; while ((float)Nx < sqrt(N)) Nx++;

	for (int i=0; i<N; i++) 
	{
		cout << "Extracting Image No: " << (i+1) << "\n";
		ix = 1.0 + (float)(i / Nx);
		iy = 1.0 + (float)(i % Nx);
		im_output.readBmp(argv[1]);
		im_output.applyBandpassFilter((2.0 * ix - 1.0) * PI / (float)(2 * Nx), (2.0 * iy - 1.0) * PI / (float)(2 * Nx), PI / (float)(2 * Nx));
		im_output.applySpectogramShift((2.0 * ix - 1.0) * PI / (float)(2 * Nx), (2.0 * iy - 1.0) * PI / (float)(2 * Nx));
		im_output.applyLowpassFilter(PI / (float)(2 * Nx));
		sprintf(filename,"output%d.bmp", i+1);
		im_output.writeBmp(filename);
	}
}

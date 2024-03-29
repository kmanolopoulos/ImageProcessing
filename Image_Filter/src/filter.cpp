#include <iostream>
#include <math.h>
#include "LibInclude.h"

using namespace std;

int main(int argc,char **argv)
{
	ColorImage im;
	Filter filtX(PREWITT_X, 7);
	Filter filtY(PREWITT_Y, 7);
	
	if (argc != 2) 
	{
		cout << "Usage: " << argv[0] << " <input_image>\n";
		exit(0);
	}

	im.readBmp(argv[1]);
	im.applyFilter(filtX);
	im.writeBmp("outputX.bmp");
	
	im.readBmp(argv[1]);
	im.applyFilter(filtY);
	im.writeBmp("outputY.bmp");
}

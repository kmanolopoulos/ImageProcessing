#include <iostream>
#include <math.h>
#include "LibInclude.h"

using namespace std;

int main(int argc,char **argv)
{
	ColorImage im;
	Filter filt(PREWITT);
	
	if (argc != 2) 
	{
		cout << "Usage: " << argv[0] << " <input_image>\n";
		exit(0);
	}

	im.readBmp(argv[1]);
	im.ApplyFilter(filt);
	im.writeBmp("output.bmp");
}

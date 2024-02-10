#include <iostream>
#include "LibInclude.h"

using namespace std;

int main(int argc, char **argv)
{
	BwImage im;

	if (argc != 2) 
	{
		cout << "Usage: " << argv[0] << " <input_image>\n";
		exit(0);
	}

	im.readBmp(argv[1]);
  
	im.Denoise();
  
	im.writeBmp("output.bmp");
}
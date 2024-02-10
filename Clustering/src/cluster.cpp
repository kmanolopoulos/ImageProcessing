#include <iostream>
#include <math.h>
#include "LibInclude.h"

using namespace std;

void Cluster(ColorImage &im, float R,float G,float B,int level)
{
	ColorPixel pixel = { 0.0, 0.0, 0.0 };
	int lines, columns;
	float mx,my;
	float sx,sy;
	float rxy;
	
	im.getSize(lines, columns);

	cout << "Applying clustering ... ";
	cout.flush();
  
	my = (R+G+B) / 3;
	sy = sqrt((R-my)*(R-my) + (G-my)*(G-my) + (B-my)*(B-my));
  
	for (int i=0;i<lines;i++) 
	{
		for (int j=0;j<columns;j++) 
		{
			pixel = im.getPixel(i, j);
			
			mx = (pixel.RComponent + pixel.GComponent + pixel.BComponent) / 3;
			sx = sqrt((pixel.RComponent - mx) * (pixel.RComponent - mx) +
					  (pixel.GComponent - mx) * (pixel.GComponent - mx) + 
					  (pixel.BComponent - mx) * (pixel.BComponent - mx));

			rxy = (pixel.RComponent - mx) * (R - my) + (pixel.GComponent - mx) * (G - my) + (pixel.BComponent - mx) * (B - my);
      
			rxy /= (sx*sy);
			
			pixel.RComponent = pow(rxy + 1.0,(double)level);
			pixel.GComponent = pow(rxy + 1.0,(double)level);
			pixel.BComponent = pow(rxy + 1.0,(double)level);
			
			im.setPixel(i, j, pixel);
		}
	}

	cout << "Done\n";
}

//
// Starting of execution
//
int main(int argc, char **argv)
{
	ColorImage im;
	int level;
	float R,G,B;

	if (argc != 6) 
	{
		cout << "Usage: " << argv[0] << " <input_image> <R> <G> <B> <level>\n"; 
		exit(0);
	}

	R = atof(argv[2]);
	G = atof(argv[3]);
	B = atof(argv[4]);
	level = atoi(argv[5]);
  
	im.readBmp(argv[1]);
	Cluster(im, R, G, B, level);
	im.writeBmp("output.bmp");
  
	return 0;
}

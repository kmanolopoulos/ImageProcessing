#include <iostream>
#include <fstream>
#include <string.h>
#include "GrayImage.h"
#include "BmpHeaders.h"

using namespace std;

//                            //
//                            //
// Gray_Image class routines  //
//                            //
//                            //

GrayImage::GrayImage() 
{
	lines = columns = -1;
}

GrayImage::~GrayImage() 
{
	DeAllocate();
}

void GrayImage::getSize(int &x, int &y) 
{
	x = lines;
	y = columns;
}

void GrayImage::createNew(int x, int y) 
{
	DeAllocate();
	Allocate(x,y);
}

void GrayImage::Allocate(int x, int y) 
{
	if ((x<0) || (y<0)) 
	{
		cout << "Bad Allocation size (" << x << "," << y << ")\n";
		return;
	}

	lines = x; columns = y;

	if ((pixel = new GrayPixel*[lines]) == NULL)
	{
		cout << "Out of memory\n";
		exit(1);
	}

	for (int i=0;i<lines;i++) 
	{
		if ((pixel[i] = new GrayPixel[columns]) == NULL) 
		{
			cout << "Out of memory\n";
			exit(1);
		}
	}
}

void GrayImage::DeAllocate() 
{
	if ((lines < 0) || (columns < 0)) return;

	for (int i=0; i<lines; i++) 
	{
		delete[] pixel[i];
	}
	delete[] pixel;

	lines = columns = -1;
}

void GrayImage::Normalize()
{
	//GrayPixel value;
	float minValue, maxValue, currentValue;
	
	// Locate minimum and maximum values for component
	minValue = maxValue = pixel[0][0].component;

	for (int i=0; i<lines; i++)
	{
		for (int j=0; j<columns; j++)
		{
			if (pixel[i][j].component < minValue)
				minValue = pixel[i][j].component;

			if (pixel[i][j].component > maxValue)
				maxValue = pixel[i][j].component;
		}
	}
	
	cout << "B=[" << minValue << "-" << maxValue << "] ...";
	cout.flush();
	
	// Bypass normalization if values are already in [0.255] range
	if ((minValue >= 0.0) && (maxValue <= 255.0))
	{
		return;
	}
	
	cout << "Normalizing ... ";
	cout.flush();
	
	// Perform normalization to [0.255] range
	for (int i=0; i<lines; i++)
	{
		for (int j=0; j<columns; j++)
		{
			pixel[i][j].component = 255.0 * (pixel[i][j].component - minValue) / (maxValue - minValue);
		}
	}
}

GrayPixel GrayImage::getPixel(int x,int y) 
{
	GrayPixel value = { 0.0 };

	if ((x<0) || (y<0) || (x>=lines) || (y>=columns)) 
	{
		cout << "Bad value in getPixel: (" << x << "," << y << ")\n";
		return value;
	}

	return pixel[x][y];
}

void GrayImage::setPixel(int x, int y, GrayPixel value) 
{
	if ((x<0) || (y<0) || (x>=lines) || (y>=columns)) 
	{
		cout << "Bad value in setPixel: (" << x << "," << y << ")\n";
		return;
	}

	pixel[x][y] = value;
}

void GrayImage::readBmp(const char *filename)
{
	struct Bmp_header          header1;
	struct Win3_1_Bmp_header   header2;
	unsigned char              *buf;
	ifstream                   fp;
	int                        column_size;
  
	// Deallocate previous image
	DeAllocate();

	cout << "Reading data from file: " << filename << " ... ";
	cout.flush();

	// Open Image file
	fp.open(filename,ios::in|ios::binary);
	if (!fp.is_open()) 
	{
		cout << "\nUnable to open file " << filename << "!\n";
		return;
	}

	// Read header of BMP
	fp.seekg(0,ios::beg);

	fp.read(reinterpret_cast<char *>(&header1.MagicWord),sizeof(header1.MagicWord));
	fp.read(reinterpret_cast<char *>(&header1.filesize),sizeof(header1.filesize));
	fp.read(reinterpret_cast<char *>(&header1.reserved),sizeof(header1.reserved));
	fp.read(reinterpret_cast<char *>(&header1.bmpoffset),sizeof(header1.bmpoffset));
	fp.read(reinterpret_cast<char *>(&header1.bmpinfoheadersize),sizeof(header1.bmpinfoheadersize));

	if (strncmp(header1.MagicWord,"BM",2)) 
	{
		cout << "\nBad magic word!\n";
		fp.close();
		return;
	}

	if (header1.bmpinfoheadersize != 40) 
	{
		cout << "\nNot supported bmp image\n";
		fp.close();
		return;
	}

	// Read Windows 3.1 Header
	fp.read(reinterpret_cast<char *>(&header2.X_size),sizeof(header2.X_size));
	fp.read(reinterpret_cast<char *>(&header2.Y_size),sizeof(header2.Y_size));
	fp.read(reinterpret_cast<char *>(&header2.planes),sizeof(header2.planes));
	fp.read(reinterpret_cast<char *>(&header2.bpp),sizeof(header2.bpp));
	fp.read(reinterpret_cast<char *>(&header2.compression_type),sizeof(header2.compression_type));
	fp.read(reinterpret_cast<char *>(&header2.picturesize),sizeof(header2.picturesize));
	fp.read(reinterpret_cast<char *>(&header2.X_resolution),sizeof(header2.X_resolution));
	fp.read(reinterpret_cast<char *>(&header2.Y_resolution),sizeof(header2.Y_resolution));
	fp.read(reinterpret_cast<char *>(&header2.used_colors),sizeof(header2.used_colors));
	fp.read(reinterpret_cast<char *>(&header2.important_colors),sizeof(header2.important_colors));

	if (header2.planes != 1) 
	{
		cout << "\nNumber of planes must be 1\n";
		fp.close();
		return;
	}

	if (header2.bpp != 8)
	{
		cout << "\nThis is not an 8 bits/pixel color image. Aborting...\n";
		fp.close();
		return;
	}

	if (header2.compression_type != 0) 
	{
		cout << "\nNot supporting compressed images\n";
		fp.close();
		return;
	}

	// Read data and store it to memory
	createNew((int)header2.Y_size,(int)header2.X_size);

	fp.seekg(header1.bmpoffset,ios::beg);

	column_size = 4 * ((8 * columns + 31) / 32);
	buf = new unsigned char [column_size];

	for (int i=lines-1; i>=0; i--) 
	{
		fp.read(reinterpret_cast<char *>(buf),column_size);
		for (int j=0;j<columns;j++) 
		{
			pixel[i][j].component = (float)buf[j];
		}
	}

	delete[] buf;

	// Close Image file
	fp.close();

	cout << "Done\n";
}

void GrayImage::writeBmp(const char *filename)
{
	ofstream                   fp;
	uint32_t                   filesize;
	unsigned char              *buf;
	struct Bmp_header          header1;
	struct Win3_1_Bmp_header   header2;
	int                        column_size;

	if ((lines < 0) || (columns < 0)) 
	{
		cout << "There is no image to write to disk\n";
		return;
	}

	cout << "Writing data to file: " << filename << " ... ";
	cout.flush();

	// Open Image file
	fp.open(filename,ios::out|ios::binary|ios::trunc);
	if (!fp.is_open()) 
	{
		cout << "\nUnable to open file " << filename << "!\n";
		return;
	}
	
	column_size = 4 * ((8 * columns + 31) / 32);

	// Write header of BMP
	strcpy(header1.MagicWord,"BM");
	fp.write(reinterpret_cast<char *>(&header1.MagicWord),sizeof(header1.MagicWord));
	header1.filesize  = (uint32_t)(lines*column_size + 1078);
	fp.write(reinterpret_cast<char *>(&header1.filesize),sizeof(header1.filesize));
	strncpy(header1.reserved,"\0\0\0\0",4);
	fp.write(reinterpret_cast<char *>(&header1.reserved),sizeof(header1.reserved));
	header1.bmpoffset = 1078L;
	fp.write(reinterpret_cast<char *>(&header1.bmpoffset),sizeof(header1.bmpoffset));
	header1.bmpinfoheadersize = 40L;
	fp.write(reinterpret_cast<char *>(&header1.bmpinfoheadersize),sizeof(header1.bmpinfoheadersize));

	header2.X_size  = (uint32_t)columns;
	fp.write(reinterpret_cast<char *>(&header2.X_size),sizeof(header2.X_size));
	header2.Y_size  = (uint32_t)lines;
	fp.write(reinterpret_cast<char *>(&header2.Y_size),sizeof(header2.Y_size));
	header2.planes  = 1;
	fp.write(reinterpret_cast<char *>(&header2.planes),sizeof(header2.planes));
	header2.bpp     = 8;
	fp.write(reinterpret_cast<char *>(&header2.bpp),sizeof(header2.bpp));
	header2.compression_type = 0L;
	fp.write(reinterpret_cast<char *>(&header2.compression_type),sizeof(header2.compression_type));
	header2.picturesize     = header1.filesize - header1.bmpoffset;
	fp.write(reinterpret_cast<char *>(&header2.picturesize),sizeof(header2.picturesize));
	header2.X_resolution    = 0;
	fp.write(reinterpret_cast<char *>(&header2.X_resolution),sizeof(header2.X_resolution));
	header2.Y_resolution    = 0;
	fp.write(reinterpret_cast<char *>(&header2.Y_resolution),sizeof(header2.Y_resolution));
	header2.used_colors     = 0L;
	fp.write(reinterpret_cast<char *>(&header2.used_colors),sizeof(header2.used_colors));
	header2.important_colors = 0L;
	fp.write(reinterpret_cast<char *>(&header2.important_colors),sizeof(header2.important_colors));
	
	uint32_t currentColor;
	
	for (int i=0; i<=255; i++)
	{
		currentColor = i + (i << 8) + (i << 16);
		fp.write(reinterpret_cast<char *>(&currentColor),sizeof(currentColor));
	}
	
	// Normalize pixels to 0..255
	Normalize();
  
	// Write data into disk
	buf = new unsigned char [column_size];

	for (int i=lines-1; i>=0; i--) 
	{
		for (int j=0; j<columns; j++) 
		{
			buf[j] = (unsigned char)(int)pixel[i][j].component;
		}

		for (int j=columns; j<column_size; j++) 
		{
			buf[j] = 0;
		}
		fp.write(reinterpret_cast<char *>(buf),column_size);
	}

	delete[] buf;
  
	// Close Image file
	fp.close();

	cout << "Done\n";
}

void GrayImage::ApplyFilter(Filter &f)
{
	int length;
	int xindex, yindex;
	float valueC;
	GrayPixel filteredPixel = { 0.0 };
	GrayImage filtered;
	
	cout << "Applying Filter ... ";
	cout.flush();
	
	f.GetSize(length);
	filtered.createNew(lines, columns);
	
	for (int i=0;i<lines;i++) 
	{
		for (int j=0;j<columns;j++)
		{
			valueC = 0.0;

			for (int x=0;x<length;x++) 
			{
				for (int y=0;y<length;y++) 
				{
					xindex = i-x+length/2; yindex = j-y+length/2;
					if (xindex < 0)        xindex = -xindex-1;
					if (xindex >= lines)   xindex = 2*lines-xindex-1;
					if (yindex < 0)        yindex = -yindex-1;
					if (yindex >= columns) yindex = 2*columns-yindex-1;
					
					valueC += f.GetValue(x,y) * pixel[xindex][yindex].component;
				}
			}
			
			if (valueC < 0.0) valueC = - valueC;
			
			filteredPixel.component = valueC;
			filtered.setPixel(i, j, filteredPixel);
		}
	}

	for (int i=0;i<lines;i++) 
	{
		for (int j=0;j<columns;j++) 
		{
			pixel[i][j] = filtered.getPixel(i, j);
		}
	}
	
	cout << "Done\n";
	return;

}

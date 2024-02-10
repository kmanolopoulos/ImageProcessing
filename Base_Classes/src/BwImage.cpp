#include <iostream>
#include <fstream>
#include <string.h>
#include "BwImage.h"
#include "BmpHeaders.h"

using namespace std;

//                          //
//                          //
// BW_Image class routines  //
//                          //
//                          //

BwImage::BwImage() 
{
	lines = columns = -1;
}

BwImage::~BwImage() 
{
	DeAllocate();
}

void BwImage::getSize(int &x, int &y) 
{
	x = lines;
	y = columns;
}

void BwImage::createNew(int x, int y) 
{
	DeAllocate();
	Allocate(x,y);
}

void BwImage::Allocate(int x, int y) 
{
	if ((x<0) || (y<0)) 
	{
		cout << "Bad Allocation size (" << x << "," << y << ")\n";
		return;
	}

	lines = x; columns = y;

	if ((pixel = new BwPixel*[lines]) == NULL)
	{
		cout << "Out of memory\n";
		exit(1);
	}

	for (int i=0;i<lines;i++) 
	{
		if ((pixel[i] = new BwPixel[columns]) == NULL) 
		{
			cout << "Out of memory\n";
			exit(1);
		}
	}
}

void BwImage::DeAllocate() 
{
	if ((lines < 0) || (columns < 0)) return;

	for (int i=0; i<lines; i++) 
	{
		delete[] pixel[i];
	}
	delete[] pixel;

	lines = columns = -1;
}

BwPixel BwImage::getPixel(int x,int y) 
{
	BwPixel value = { 0 };

	if ((x<0) || (y<0) || (x>=lines) || (y>=columns)) 
	{
		cout << "Bad value in getPixel: (" << x << "," << y << ")\n";
		return value;
	}

	return pixel[x][y];
}

void BwImage::setPixel(int x, int y, BwPixel value) 
{
	if ((x<0) || (y<0) || (x>=lines) || (y>=columns)) 
	{
		cout << "Bad value in setPixel: (" << x << "," << y << ")\n";
		return;
	}

	pixel[x][y] = value;
}

void BwImage::readBmp(const char *filename)
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

	if (header2.bpp != 1) 
	{
		cout << "\nThis is not an 1 bits/pixel color image. Aborting...\n";
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

	column_size = 4 * ((1 * columns + 31) / 32);
	buf = new unsigned char [column_size];

	for (int i=lines-1; i>=0; i--) 
	{
		fp.read(reinterpret_cast<char *>(buf),column_size);
		for (int j=0; j<columns; j++) 
		{
			pixel[i][j].component = (buf[j/8] & (128 >> (j % 8))) >> (7 - (j % 8));
		}
	}

	delete[] buf;
	
	// Close Image file
	fp.close();

	cout << "Done\n";
}

void BwImage::writeBmp(const char *filename) 
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
	
	column_size = 4 * ((1 * columns + 31) / 32);

	// Write header of BMP
	strcpy(header1.MagicWord,"BM");
	fp.write(reinterpret_cast<char *>(&header1.MagicWord),sizeof(header1.MagicWord));
	header1.filesize  = (uint32_t)(lines*column_size + 62);
	fp.write(reinterpret_cast<char *>(&header1.filesize),sizeof(header1.filesize));
	strncpy(header1.reserved,"\0\0\0\0",4);
	fp.write(reinterpret_cast<char *>(&header1.reserved),sizeof(header1.reserved));
	header1.bmpoffset = 62L;
	fp.write(reinterpret_cast<char *>(&header1.bmpoffset),sizeof(header1.bmpoffset));
	header1.bmpinfoheadersize = 40L;
	fp.write(reinterpret_cast<char *>(&header1.bmpinfoheadersize),sizeof(header1.bmpinfoheadersize));

	header2.X_size  = (uint32_t)columns;
	fp.write(reinterpret_cast<char *>(&header2.X_size),sizeof(header2.X_size));
	header2.Y_size  = (uint32_t)lines;
	fp.write(reinterpret_cast<char *>(&header2.Y_size),sizeof(header2.Y_size));
	header2.planes  = 1;
	fp.write(reinterpret_cast<char *>(&header2.planes),sizeof(header2.planes));
	header2.bpp     = 1;
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

	uint32_t color1 = 0x00000000;
	uint32_t color2 = 0x00ffffff;
  
	fp.write(reinterpret_cast<char *>(&color1),sizeof(color1));
	fp.write(reinterpret_cast<char *>(&color2),sizeof(color2));
  
	// Write data into disk
	buf = new unsigned char [column_size];

	for (int i=lines-1; i>=0; i--) 
	{
		for (int j=0;j<column_size;j++) 
		{
			buf[j] = 0;
		}

		for (int j=0;j<columns;j++) 
		{
			buf[j/8] |= (unsigned char)(pixel[i][j].component << (7 - (j % 8)));
		}
		fp.write(reinterpret_cast<char *>(buf),column_size);
	}

	delete[] buf;
  
	// Close Image file
	fp.close();

	cout << "Done\n";
}

void BwImage::Denoise()
{
	int changed;
	int **current_label;
	int **next_label;
	int **current_state;
	int **next_state;
	int c0,c1;
	
	// Mark all pixels are subject to change
	changed = lines * columns;

	// Allocate memory for variables
	current_label = new int * [lines];
	for (int i=0;i<lines;i++) current_label[i] = new int [columns];
	next_label = new int * [lines];
	for (int i=0;i<lines;i++) next_label[i] = new int [columns];
	current_state = new int * [lines];
	for (int i=0;i<lines;i++) current_state[i] = new int [columns];
	next_state = new int * [lines];
	for (int i=0;i<lines;i++) next_state[i] = new int [columns];
  
	// Check all pixels on first time
	for (int i=0;i<lines;i++) 
	{
		for (int j=0;j<columns;j++) 
		{
			next_label[i][j] = pixel[i][j].component;
			next_state[i][j] = 1;
		}
	}
  
	// Loop while at least one pixel is subject to change
	while (changed) 
	{
		changed = 0;

		// Update pixels statuses
		for (int i=0;i<lines;i++) 
		{
			for (int j=0;j<columns;j++) 
			{
				current_label[i][j] = next_label[i][j];
				current_state[i][j] = next_state[i][j];
				next_state[i][j] = 0;
			}
		}
    
		// Check active pixels
		for (int i=0;i<lines;i++) 
		{
			for (int j=0;j<columns;j++) 
			{
				if (current_state[i][j] == 1) 
				{
					c0 = c1 = 0;
					if ((i-1 >= 0) && (j-1 >= 0)) 
					{
						if (current_label[i-1][j-1] == 0) c0++;
						else c1++;
					}
					if (j-1 >= 0) 
					{
						if (current_label[i][j-1] == 0) c0++;
						else c1++;
					}
					if ((i+1 < lines) && (j-1 >= 0)) 
					{
						if (current_label[i+1][j-1] == 0) c0++;
						else c1++;
					}
					if (i-1 >= 0) 
					{
						if (current_label[i-1][j] == 0) c0++;
						else c1++;
					}
					if (i+1 < lines) 
					{
						if (current_label[i+1][j] == 0) c0++;
						else c1++;
					}
					if ((i-1 >= 0) && (j+1 < columns)) 
					{
						if (current_label[i-1][j+1] == 0) c0++;
						else c1++;
					}
					if (j+1 < columns) 
					{
						if (current_label[i][j+1] == 0) c0++;
						else c1++;
					}
					if ((i+1 < lines) && (j+1 < columns)) 
					{
						if (current_label[i+1][j+1] == 0) c0++;
						else c1++;
					}

					if (2*c0 <= c1) next_label[i][j] = 1;
					else if (2*c1 <= c0) next_label[i][j] = 0;
					else next_label[i][j] = current_label[i][j];
	    
					if (next_label[i][j] != current_label[i][j]) 
					{
						changed++;
						if (i-1 >= 0) next_state[i-1][j] = 1;
						if (i+1 < lines) next_state[i+1][j] = 1;
						if (j-1 >= 0) next_state[i][j-1] = 1;
						if (j+1 < columns) next_state[i][j+1] = 1;
						if ((i-1 >= 0)&&(j-1 >= 0)) next_state[i-1][j-1] = 1;
						if ((i-1 >= 0)&&(j+1 < columns)) next_state[i-1][j+1] = 1;
						if ((i+1 < lines)&&(j-1 >= 0)) next_state[i+1][j-1] = 1;
						if ((i+1 < lines)&&(j+1 < columns)) next_state[i+1][j+1] = 1;
					}
	     
				}
			}
		}
		cout << changed << " pixels changed value\n"; 
	}
  
	// Give new values to pixels changed
	for (int i=0;i<lines;i++) 
	{
		for (int j=0;j<columns;j++) 
		{
			pixel[i][j].component = next_label[i][j];
		}
	}

	// Deallocate memory of variables
	for (int i=0;i<lines;i++) delete[] current_label[i];
	delete[] current_label;
	for (int i=0;i<lines;i++) delete[] next_label[i];
	delete[] next_label;
	for (int i=0;i<lines;i++) delete[] current_state[i];
	delete current_state;
	for (int i=0;i<lines;i++) delete[] next_state[i];
	delete[] next_state;
}


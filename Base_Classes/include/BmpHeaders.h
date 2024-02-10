#ifndef __BMPHEADERS_H__
#define __BMPHEADERS_H__

//
// .bmp Headers
//
struct Bmp_header 
{
	char     MagicWord[2];        // Must be 'BM'
	uint32_t filesize;            // Size of all File in bytes
	char     reserved[4];         // Reserved for future use
	uint32_t bmpoffset;           // offset of data in file
	uint32_t bmpinfoheadersize;   // Must be 40 to be ok(Windows 3.1 bmp header)
};

struct Win3_1_Bmp_header 
{
	uint32_t X_size;             // number of lines
	uint32_t Y_size;             // number of columns
	uint16_t planes;             // number of planes
	uint16_t bpp;                // bits per pixel(24 for color images)
	uint32_t compression_type;   // 0 = No compression
	uint32_t picturesize;        // size of picture in bytes = filesize-headersize
	uint32_t X_resolution;       // Horizontial resolution
	uint32_t Y_resolution;       // Vertical resolution
	uint32_t used_colors;        // number of used colors
	uint32_t important_colors;   // number of important colors
};

#endif	//__BMPHEADERS_H__

#include <iostream.h>
#define BLOCKSIZE 8

void Encode(unsigned char *data,int size,FILE *fp)
{
  int p[256];

  for (int i=0;i<256;i++) p[i] = 0;

  for (int i=0;i<size;i++) {
    ++p[data[i]];
  }
  
}

void Huffman(Image &im,FILE *fp)
{
  int lines,columns;

  im.getSize(lines,columns);
  
  data = new unsigned char [3*BLOCKSIZE*BLOCKSIZE];

  for (int i=0;i<lines/BLOCKSIZE;i++) {
    for (int j=0;j<columns/BLOCKSIZE;j++) {
      for (int color=0;color<3;color++) {
        for (int k=0;k<BLOCKSIZE;k++) {
          for (int l=0;l<BLOCKSIZE;l++) {
            data[color*BLOCKSIZE*BLOCKSIZE+k*BLOCKSIZE+l] = 
              (unsigned char)getPixel(i*BLOCKSIZE+k,j*BLOCKSIZE+l,color);
	  }
	}
      }
      Encode(data,3*BLOCKSIZE*BLOCKSIZE,fp);
    }
  }

  for (int color=0;color<3;color++) {
    for (int k=0;k<lines-(lines/BLOCKSIZE)*BLOCKSIZE;k++) {
      for (int l=0;l<columns-(columns/BLOCKSIZE)*BLOCKSIZE;l++) {
        data[color*BLOCKSIZE*BLOCKSIZE+k*BLOCKSIZE+l] = 
          (unsigned char)getPixel((lines/BLOCKSIZE)*BLOCKSIZE+k,
                                  (columns/BLOCKSIZE)*BLOCKSIZE+l,
                                  color);
      }
    }
  }
  Encode(data,(lines-(lines/BLOCKSIZE)*BLOCKSIZE)*
              (columns-(columns/BLOCKSIZE)*BLOCKSIZE),fp);

  
  delete[] data;
}

int main(int argc,char **argv)
{
  Image  im;
  char   outfilename[256];
  FILE   *outf;

  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <input_image>\n";
    exit(0);
  }

  sprintf(outfilename,"%s.huf",argv[1]);
  outf = fopen(outfilename,"wb");
  if (outf == NULL) {
    cout << "Cannot open output file\n";
    exit(1);
  }
  
  im.readBmp(argv[1]);
  Huffman(im,outf);

  fclose(outf);
  
  return 0;
}

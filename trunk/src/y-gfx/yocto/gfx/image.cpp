#include "yocto/gfx/image.hpp"

#define __putc(C,F) F.write(C)

namespace yocto
{
    namespace gfx
    {
        static inline
        void BM_WriteHexString(ios::ostream &fptr,const char *s)
        {
            unsigned int i;
            int c;
            char hex[3];
            
            for (i=0;i<strlen(s);i+=2) {
                hex[0] = s[i];
                hex[1] = s[i+1];
                hex[2] = '\0';
                sscanf(hex,"%X",&c);
                __putc(c,fptr);
            }
        }
        
        void BM_WriteLongInt(ios::ostream &fptr,char *s,long n)
        {
            int i;
            
            s[0] = (n & 0xff000000) / 16777216;
            s[1] = (n & 0x00ff0000) / 65536;
            s[2] = (n & 0x0000ff00) / 256;
            s[3] = (n & 0x000000ff);
            
            for (i=0;i<4;i++)
                __putc(s[i],fptr);
        }
        
        
        /*
         Write a bitmap to a file
         The format is as follows
         1 == tga
         11 == tga with alpha
         12 == compressed tga
         13 == compressed tga with alpha
         2 == ppm
         3 == rgb
         4 == raw grey scale
         5 == tiff
         6 == EPS colour (Encapsulated PostScript)
         7 == EPS black and white
         8 == raw
         9 == BMP
         A negative format indicates a vertical flip
         */
        
        
        void image:: save(ios::ostream &fptr,
                          const bitmap &bmp,
                          int           fmt,
                          addr2rgba     proc,
                          const void   *args)
        {
            assert(args);
            assert(proc);
            const int nx  = bmp.w;
            const int ny  = bmp.h;
            const int FMT = abs(fmt);
            int offset=0;
            int size=0;
            char  buffer[1024];
            
            /* Write the header */
            switch(FMT) {
                case 1:
                case 11:
                case 12:
                case 13:
                    __putc(0,fptr);  /* Length of ID */
                    __putc(0,fptr);  /* No colour map */
                    if (FMT == 12 || FMT == 13)
                        __putc(10,fptr); /* compressed RGB */
                    else
                        __putc(2,fptr); /* uncompressed RGB  */
                    __putc(0,fptr); /* Index of colour map entry */
                    __putc(0,fptr);
                    __putc(0,fptr); /* Colour map length */
                    __putc(0,fptr);
                    __putc(0,fptr); /* Colour map size */
                    __putc(0,fptr); /* X origin */
                    __putc(0,fptr);
                    __putc(0,fptr); /* Y origin */
                    __putc(0,fptr);
                    __putc((nx & 0x00ff),fptr); /* X width */
                    __putc((nx & 0xff00) / 256,fptr);
                    __putc((ny & 0x00ff),fptr); /* Y width */
                    __putc((ny & 0xff00) / 256,fptr);
                    if (FMT == 11 || FMT == 13) {
                        __putc(32,fptr);                      /* 32 bit bitmap     */
                        __putc(0x08,fptr);
                    } else {
                        __putc(24,fptr);                 		/* 24 bit bitmap 		*/
                        __putc(0x00,fptr);
                    }
                    break;
                case 2:
                    fptr("P6\n%d %d\n255\n",nx,ny);
                    break;
                case 3:
                    __putc(0x01,fptr);
                    __putc(0xda,fptr);
                    __putc(0x00,fptr);
                    __putc(0x01,fptr);
                    __putc(0x00,fptr);
                    __putc(0x03,fptr);
                    __putc((nx & 0xFF00) / 256,fptr);
                    __putc((nx & 0x00FF),fptr);
                    __putc((ny & 0xFF00) / 256,fptr);
                    __putc((ny & 0x00FF),fptr);
                    BM_WriteHexString(fptr,"000300000000000000ff00000000");
                    fptr("WriteBitmap, pdb");
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    break;
                case 4:
                    break;
                case 5:
                    BM_WriteHexString(fptr,"4d4d002a");	/* Little endian & TIFF identifier */
                    offset = nx * ny * 3 + 8;
                    BM_WriteLongInt(fptr,buffer,offset);
                    break;
                case 6:
                    fptr("%%!PS-Adobe-3.0 EPSF-3.0\n");
                    fptr("%%%%Creator: Created from bitmaplib by Paul Bourke\n");
                    fptr("%%%%BoundingBox: %d %d %d %d\n",0,0,nx,ny);
                    fptr("%%%%LanguageLevel: 2\n");
                    fptr("%%%%Pages: 1\n");
                    fptr("%%%%DocumentData: Clean7Bit\n");
                    fptr("%d %d scale\n",nx,ny);
                    fptr("%d %d 8 [%d 0 0 -%d 0 %d]\n",nx,ny,nx,ny,ny);
                    fptr("{currentfile 3 %d mul string readhexstring pop} bind\n",nx);
                    fptr("false 3 colorimage\n");
                    break;
                case 7:
                    fptr("%%!PS-Adobe-3.0 EPSF-3.0\n");
                    fptr("%%%%Creator: Created from bitmaplib by Paul Bourke\n");
                    fptr("%%%%BoundingBox: %d %d %d %d\n",0,0,nx,ny);
                    fptr("%%%%LanguageLevel: 2\n");
                    fptr("%%%%Pages: 1\n");
                    fptr("%%%%DocumentData: Clean7Bit\n");
                    fptr("%d %d scale\n",nx,ny);
                    fptr("%d %d 8 [%d 0 0 -%d 0 %d]\n",nx,ny,nx,ny,ny);
                    fptr("{currentfile %d string readhexstring pop} bind\n",nx);
                    fptr("false 1 colorimage\n");
                    break;
                case 8:
                    break;
                case 9:
                    /* Header 10 bytes */
                    __putc('B',fptr);
                    __putc('M',fptr);
                    size = nx * ny * 3 + 14 + 40;
                    __putc((size) % 256,fptr);
                    __putc((size / 256) % 256,fptr);
                    __putc((size / 65536) % 256,fptr);
                    __putc((size / 16777216),fptr);
                    __putc(0,fptr); __putc(0,fptr);
                    __putc(0,fptr); __putc(0,fptr);
                    /* Offset to image data */
                    __putc(14+40,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr);
                    /* Information header 40 bytes */
                    __putc(0x28,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr);
                    __putc((nx) % 256,fptr);
                    __putc((nx / 256) % 256,fptr);
                    __putc((nx / 65536) % 256,fptr);
                    __putc((nx / 16777216),fptr);
                    __putc((ny) % 256,fptr);
                    __putc((ny / 256) % 256,fptr);
                    __putc((ny / 65536) % 256,fptr);
                    __putc((ny / 16777216),fptr);
                    __putc(1,fptr);  __putc(0,fptr); /* One plane */
                    __putc(24,fptr); __putc(0,fptr); /* 24 bits */
                    /* Compression type == 0 */
                    __putc(0,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr);
                    size = nx * ny * 3;
                    __putc((size) % 256,fptr);
                    __putc((size / 256) % 256,fptr);
                    __putc((size / 65536) % 256,fptr);
                    __putc((size / 16777216),fptr);
                    __putc(1,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr);
                    __putc(1,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr);
                    __putc(0,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr); /* No palette */
                    __putc(0,fptr); __putc(0,fptr); __putc(0,fptr); __putc(0,fptr);
                    break;
            }
            
        }
        
        
    }
}


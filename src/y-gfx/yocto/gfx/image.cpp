#include "yocto/gfx/image.hpp"
#include "yocto/ios/ocstream.hpp"

#define __putc(C,F) F.write(C)

namespace yocto
{
    namespace gfx
    {
        
        void image::save(const string &filename,
                         const bitmap &bmp,
                         const format_t fmt,
                         addr2rgba     &proc,
                         bool           flip )
        {
            ios::ocstream fp(filename,false);
            save(fp, bmp, fmt, proc, flip);
        }
        
        
        static inline
        void BM_WriteHexString(ios::ostream &fptr,const char *s)
        {
            assert(s!=0);
            const unsigned n = strlen(s);
            char hex[4] = {0,0,0,0};
            
            for(unsigned i=0;i<n;i+=2)
            {
                hex[0] = s[i];
                hex[1] = s[i+1];
                unsigned c = 0;
                sscanf(hex,"%X",&c);
                __putc(c,fptr);
            }
        }
        
        static inline uint8_t __greyscale( const rgb_t &C ) throw()
        {
            const float r = conv::unit_float[C.r];
            const float g = conv::unit_float[C.g];
            const float b = conv::unit_float[C.b];
            const float Y = 0.2126*r + 0.7152*g + 0.0722*b;
            return conv::to_byte(Y);
        }
        
        void BM_WriteLongInt(ios::ostream &fptr, long n)
        {
            char s[4];
            s[0] = (n & 0xff000000) / 16777216;
            s[1] = (n & 0x00ff0000) / 65536;
            s[2] = (n & 0x0000ff00) / 256;
            s[3] = (n & 0x000000ff);
            for(int i=0;i<4;i++)
                __putc(s[i],fptr);
        }
        
        
        static inline bool Same_BitmapPixel(const rgb_t &lhs, const rgb_t &rhs ) throw()
        {
            if(lhs.r!=rhs.r) return false;
            if(lhs.g!=rhs.g) return false;
            if(lhs.b!=rhs.b) return false;
            if(lhs.a!=rhs.a) return false;
            return true;
        }
        
        /*
         Write a compressed TGA row
         Depth is either 3 or 4
         */
        void WriteTGACompressedRow(ios::ostream &fptr,
                                   const bitmap &bmp,
                                   const unit_t  y,
                                   const int     depth,
                                   addr2rgba    &proc)
        {
            const unit_t width = bmp.w;
            int   i;
            int   counter      = 1;
            int   pixelstart   = 0;
            int   packettype   = 0;
            int   readytowrite = 0;
            rgb_t currentpixel(0,0,0,0);
            rgb_t nextpixel(0,0,0,0);
            
            currentpixel = proc(bmp.get(0,y));
            for (;;) {
                if (pixelstart+counter >= width)  // Added April to fix strange bug
                    readytowrite = 1;
                else
                    nextpixel = proc(bmp.get(pixelstart+counter,y));
                
                if (!readytowrite) {
                    if (Same_BitmapPixel(currentpixel,nextpixel)) {
                        if (packettype == 0) {
                            counter++;
                            if (counter >= 128 || (pixelstart + counter) >= width)
                                readytowrite = 1;
                        } else {
                            counter--;
                            readytowrite = 1;
                        }
                    } else {
                        if (packettype == 1 || counter <= 1) {
                            packettype = 1;
                            currentpixel = nextpixel;
                            counter++;
                            if (counter >= 128 || (pixelstart + counter) >= width)
                                readytowrite = 1;
                        } else {
                            readytowrite = 1;
                        }
                    }
                }
                
                if (readytowrite) {
                    if (pixelstart + counter > width)
                        counter = width - pixelstart;
                    if (packettype == 0) {
                        __putc(((counter-1) | 0x80),fptr);
                        __putc(currentpixel.b,fptr);
                        __putc(currentpixel.g,fptr);
                        __putc(currentpixel.r,fptr);
                        if (depth == 4)
                            __putc(currentpixel.a,fptr);
                        currentpixel = nextpixel;
                    } else {
                        __putc(counter-1,fptr);
                        for (i=0;i<counter;i++) {
                            const rgb_t tmp = proc( bmp.get(pixelstart+i,y) );
                            __putc(tmp.b,fptr);
                            __putc(tmp.g,fptr);
                            __putc(tmp.r,fptr);
                            if (depth == 4)
                                __putc(tmp.a,fptr);
                        }
                    }
                    if ((pixelstart = pixelstart + counter) >= width)
                        break; /* From for (;;) */
                    readytowrite = 0;
                    packettype = 0;
                    counter = 1;
                }
            }
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
         */
        
        
        void image:: save(ios::ostream  &fptr,
                          const bitmap  &bmp,
                          const format_t fmt,
                          addr2rgba     &proc,
                          bool           flip)
        {
            const int nx  = bmp.w;
            const int ny  = bmp.h;
            int offset=0;
            int size=0;
            
            //__________________________________________________________________
            //
            // Write the header
            //__________________________________________________________________
            
            switch(fmt) {
                case TGA:
                case TGA_A:
                case TGA_Z:
                case TGA_ZA:
                    __putc(0,fptr);  /* Length of ID */
                    __putc(0,fptr);  /* No colour map */
                    if (fmt == TGA_Z || fmt == TGA_ZA)
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
                    if (fmt == TGA_A || fmt == TGA_ZA) {
                        __putc(32,fptr);                        /* 32 bit bitmap     */
                        __putc(0x08,fptr);
                    } else {
                        __putc(24,fptr);                 		/* 24 bit bitmap 		*/
                        __putc(0x00,fptr);
                    }
                    break;
                case PPM:
                    fptr("P6\n%d %d\n255\n",nx,ny);
                    break;
                case RGB:
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
                case RAW_BW:
                    break;
                case TIFF:
                    BM_WriteHexString(fptr,"4d4d002a");	/* Little endian & TIFF identifier */
                    offset = nx * ny * 3 + 8;
                    BM_WriteLongInt(fptr,offset);
                    break;
                case EPS:
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
                case EPS_BW:
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
                case RAW:
                    break;
                case BMP:
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
            
            
            //__________________________________________________________________
            //
            // Write the binary data
            //__________________________________________________________________
            const unit_t top  = ny-1;
            unit_t linelength = 0;
            for(unit_t j=0;j<ny;j++) {
                //--------------------------------------------------------------
                //-- set default value
                //--------------------------------------------------------------
                unit_t rowindex = j;
                
                //--------------------------------------------------------------
                //-- adjust depending on format
                //--------------------------------------------------------------
                switch(fmt)
                {
                    default:
                        break;
                }
                
                //--------------------------------------------------------------
                //-- use flip
                //--------------------------------------------------------------
                if(flip)
                {
                    rowindex = top-rowindex;
                }

                switch(fmt)
                {
                    case TGA_Z:
                        WriteTGACompressedRow(fptr,bmp,rowindex,3,proc);
                        break;
                    case TGA_ZA:
                        WriteTGACompressedRow(fptr,bmp,rowindex,4,proc);
                        break;
                    default:
                        break;
                }
                
                for(unit_t i=0;i<nx;i++)
                {
                    const rgb_t C = proc( bmp.get(i,rowindex) );
                    switch (fmt) {
                        case TGA:
                        case TGA_A:
                        case BMP:
                            __putc(C.b,fptr);
                            __putc(C.g,fptr);
                            __putc(C.r,fptr);
                            if(fmt == TGA_A)
                                __putc(C.a,fptr);
                            break;
                        case PPM:
                        case RGB:
                        case TIFF:
                        case RAW:
                            __putc(C.r,fptr);
                            __putc(C.g,fptr);
                            __putc(C.b,fptr);
                            break;
                        case RAW_BW:
                            __putc(__greyscale(C),fptr);
                            break;
                        case EPS:
                            fptr("%02x%02x%02x",C.r,C.g,C.b);
                            linelength += 6;
                            if (linelength >= 72 || linelength >= nx)
                            {
                                fptr("\n");
                                linelength = 0;
                            }
                            break;
                        case EPS_BW:
                            fptr("%02x",__greyscale(C));
                            linelength += 2;
                            if (linelength >= 72 || linelength >= nx) {
                                fptr("\n");
                                linelength = 0;
                            }
                            break;
                            
                        default:
                            assert(fmt==TGA_Z||fmt==TGA_ZA);
                            break;
                    }
                }
            }
            
            
            //__________________________________________________________________
            //
            // Write the footer
            //__________________________________________________________________
            switch(fmt) {
                case TGA:
                case TGA_A:
                case TGA_Z:
                case TGA_ZA:
                case PPM:
                case RGB:
                case RAW_BW:
                    break;
                case TIFF:
                    __putc(0x00,fptr); /* The number of directory entries (14) */
                    __putc(0x0e,fptr);
                    
                    /* Width tag, short int */
                    BM_WriteHexString(fptr,"0100000300000001");
                    __putc((nx & 0xff00) / 256,fptr);		/* Image width */
                    __putc((nx & 0x00ff),fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    
                    /* Height tag, short int */
                    BM_WriteHexString(fptr,"0101000300000001");
                    __putc((ny & 0xff00) / 256,fptr);    /* Image height */
                    __putc((ny & 0x00ff),fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    
                    /* bits per sample tag, short int */
                    BM_WriteHexString(fptr,"0102000300000003");
                    offset = nx * ny * 3 + 182;
                    BM_WriteLongInt(fptr,offset);
                    
                    /* Compression flag, short int */
                    BM_WriteHexString(fptr,"010300030000000100010000");
                    
                    /* Photometric interpolation tag, short int */
                    BM_WriteHexString(fptr,"010600030000000100020000");
                    
                    /* Strip offset tag, long int */
                    BM_WriteHexString(fptr,"011100040000000100000008");
                    
                    /* Orientation flag, short int */
                    BM_WriteHexString(fptr,"011200030000000100010000");
                    
                    /* Sample per pixel tag, short int */
                    BM_WriteHexString(fptr,"011500030000000100030000");
                    
                    /* Rows per strip tag, short int */
                    BM_WriteHexString(fptr,"0116000300000001");
                    __putc((ny & 0xff00) / 256,fptr);
                    __putc((ny & 0x00ff),fptr);
                    __putc(0x00,fptr);
                    __putc(0x00,fptr);
                    
                    /* Strip byte count flag, long int */
                    BM_WriteHexString(fptr,"0117000400000001");
                    offset = nx * ny * 3;
                    BM_WriteLongInt(fptr,offset);
                    
                    /* Minimum sample value flag, short int */
                    BM_WriteHexString(fptr,"0118000300000003");
                    offset = nx * ny * 3 + 188;
                    BM_WriteLongInt(fptr,offset);
                    
                    /* Maximum sample value tag, short int */
                    BM_WriteHexString(fptr,"0119000300000003");
                    offset = nx * ny * 3 + 194;
                    BM_WriteLongInt(fptr,offset);
                    
                    /* Planar configuration tag, short int */
                    BM_WriteHexString(fptr,"011c00030000000100010000");
                    
                    /* Sample format tag, short int */
                    BM_WriteHexString(fptr,"0153000300000003");
                    offset = nx * ny * 3 + 200;
                    BM_WriteLongInt(fptr,offset);
                    
                    /* End of the directory entry */
                    BM_WriteHexString(fptr,"00000000");
                    
                    /* Bits for each colour channel */
                    BM_WriteHexString(fptr,"000800080008");
                    
                    /* Minimum value for each component */
                    BM_WriteHexString(fptr,"000000000000");
                    
                    /* Maximum value per channel */
                    BM_WriteHexString(fptr,"00ff00ff00ff");
                    
                    /* Samples per pixel for each channel */
                    BM_WriteHexString(fptr,"000100010001");
                    
                    break;
                case EPS:
                case EPS_BW:
                    fptr("\n%%%%EOF\n");
                    break;
                case RAW:
                case BMP:
                    break;
            }
            
            
        }
        
        
    }
}


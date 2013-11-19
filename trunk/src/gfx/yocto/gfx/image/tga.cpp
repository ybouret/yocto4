#include "yocto/gfx/image/tga.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/code/bzset.hpp"

namespace yocto
{
    namespace gfx
    {
        TGA:: TGA() : image::format("tga")
        {
            
        }
        
        TGA:: ~TGA() throw()
        {
        }
        
        void TGA:: save(const string  &filename,
                        const bitmap  &bmp,
                        addr2rgba     &proc,
						const char     *options) const
        {
            bitmap::iofmt fmt = bitmap::TGA;
            if(options)
            {
                
            }
            bmp.save(filename, fmt, proc);
        }
        
        namespace {
            /* 18 bytes long */
            struct TGAHEADER {
                uint8_t   idlength;
                int8_t    colourmaptype;
                int8_t    datatypecode;
                int16_t   colourmaporigin;
                int16_t   colourmaplength;
                int8_t    colourmapdepth;
                int16_t   x_origin;
                int16_t   y_origin;
                uint16_t  width;
                uint16_t  height;
                int8_t    bitsperpixel;
                int8_t    imagedescriptor;
            };
        }
        
#define __fgetc(FIELD) fptr.read1(FIELD)
        surface * TGA::load(const string      &filename,
                            const pixel_format fmt) const
        {
            ios::icstream fptr(filename);
            FILE *fp = fptr.__get();
            TGAHEADER header;
            bzset(header);
            int lo=0,hi=0;
            
            /* Read the header */
            header.idlength      = __fgetc("idlength");
            header.colourmaptype = __fgetc("coulourmaptype");
            header.datatypecode  = __fgetc("datatypecode");
            lo = __fgetc("colourmaporigin.lo");
            hi = __fgetc("colourmaporigin.hi");
            header.colourmaporigin = hi*256 + lo;
            lo = __fgetc("colourmpalength.lo");
            hi = __fgetc("colourmaplength.hi");
            header.colourmaplength = hi*256 + lo;
            header.colourmapdepth = __fgetc("colourmapdepth");
            fread(&header.x_origin,2,1,fp);
            fread(&header.y_origin,2,1,fp);
            lo = __fgetc("width.lo");
            hi = __fgetc("width.hi");
            header.width = hi*256 + lo;
            lo = __fgetc("height.lo");
            hi = __fgetc("height.hi");
            header.height = hi*256 + lo;
            header.bitsperpixel    = __fgetc("bitsperpixel");
            header.imagedescriptor = __fgetc("imagedescriptor");

            
            return 0;
        }
        
    }
}

#include "yocto/gfx/image/tga.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/auto.hpp"
#include <cerrno>

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
        
        bool TGA:: lossless() const throw()
        {
            return true;
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
            
            static inline
            void TGA_MergeBytes(rgb_t *pixel,const uint8_t *p,const int bytes)
            {
                assert(pixel);
                assert(p);
                switch(bytes)
                {
                    case 4:
                        pixel->r = p[2];
                        pixel->g = p[1];
                        pixel->b = p[0];
                        pixel->a = p[3];
                        return;
                        
                    case 3:
                        pixel->r = p[2];
                        pixel->g = p[1];
                        pixel->b = p[0];
                        pixel->a = 255;
                        return;
                        
                    case 2:
                        pixel->r = (p[1] & 0x7c) << 1;
                        pixel->g = ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
                        pixel->b = (p[0] & 0x1f) << 3;
                        pixel->a = (p[1] & 0x80);
                        return;
                        
                    case 1:
                        pixel->r = p[0];
                        pixel->g = p[0];
                        pixel->b = p[0];
                        pixel->a = 255;
                        return;
                }
            }
            
            /* 18 bytes long */
            struct TGAHEADER {
                uint8_t   idlength;
                int8_t    colourmaptype;
                int8_t    datatypecode;
                int16_t   colourmaporigin;
                int16_t   colourmaplength;
                uint8_t   colourmapdepth;
                int16_t   x_origin;
                int16_t   y_origin;
                uint16_t  width;
                uint16_t  height;
                uint8_t   bitsperpixel;
                uint8_t   imagedescriptor;
            };
            
            
        }
        
#define __fgetc(FIELD) fptr.read_char(FIELD)
#define __fgetb(FIELD) fptr.read_byte(FIELD)
        
        surface * TGA::load(const string      &filename,
                            const pixel_format fmt) const
        {
            YOCTO_GIANT_LOCK();
            ios::icstream fptr(filename);
            FILE *fp = fptr.__get();
            TGAHEADER header;
            bzset(header);
            unsigned lo=0,hi=0;
            unsigned char p[8]= {0};
            size_t bytes2read = 0;
            
            /* Read the header */
            header.idlength      = __fgetb("idlength");
            header.colourmaptype = __fgetc("coulourmaptype");
            header.datatypecode  = __fgetc("datatypecode");
            lo = __fgetb("colourmaporigin.lo");
            hi = __fgetb("colourmaporigin.hi");
            header.colourmaporigin = hi*256 + lo;
            lo = __fgetb("colourmpalength.lo");
            hi = __fgetb("colourmaplength.hi");
            header.colourmaplength = hi*256 + lo;
            header.colourmapdepth = __fgetb("colourmapdepth");
            fread(&header.x_origin,2,1,fp);
            fread(&header.y_origin,2,1,fp);
            lo = __fgetb("width.lo");
            hi = __fgetb("width.hi");
            header.width = hi*256 + lo;
            lo = __fgetb("height.lo");
            hi = __fgetb("height.hi");
            header.height = hi*256 + lo;
            header.bitsperpixel    = __fgetb("bitsperpixel");
            header.imagedescriptor = __fgetb("imagedescriptor");
            
            /*
             Can only handle image type 1, 2, 3 and 10
             1 - index colour uncompressed
             2 - rgb uncompressed
             10 - rgb rle comrpessed
             3 - grey scale uncompressed
             9 - rle index colour (unsupported)
             11 - rle black and white
             */
            if (header.datatypecode != 1 &&
                header.datatypecode != 2 &&
                header.datatypecode != 3 &&
                header.datatypecode != 11 &&
                header.datatypecode != 10) {
                throw exception("unsupported TGA datatypecode");
            }
            
            
            
            /* Can only handle pixel depths of 8, 16, 24, and 32 */
            if (header.bitsperpixel != 8  &&
                header.bitsperpixel != 16 &&
                header.bitsperpixel != 24 &&
                header.bitsperpixel != 32)
            {
                throw exception("unsupported TGA bitsperpixel");
            }
            
            /*
             Can only handle colour map types of 0 and 1
             Ignore the colour map case (1) for RGB images!
             */
            if (header.colourmaptype != 0 && header.colourmaptype != 1)
            {
                throw exception("unsupported TGA colourmaptype");
            }
            
            
            vector<rgb_t> ctable;
            /* Read the colour index table */
            if (header.datatypecode == 1)
            {
                ctable.reserve(header.colourmaplength);
                bytes2read = header.colourmapdepth / 8;
                rgb_t c;
                for(int i=0;i<header.colourmaplength;i++)
                {
                    if( fread(p,1,bytes2read,fp) != bytes2read)
                        throw exception("cannot read colour table");
                    TGA_MergeBytes(&c,p,bytes2read);
                    ctable.push_back(c);
                }
            }
            
            /* Go to the start of the image data */
            {
                int skipover = 18;
                skipover += header.idlength;
                skipover += header.colourmaptype * header.colourmaplength * header.colourmapdepth / 8;
                if( 0!= fseek(fp,skipover,SEEK_SET) )
                {
                    throw libc::exception( EIO, "TGA skip data");
                }
            }
            
            const unit_t W = header.width;
            const unit_t H = header.height;
            const unit_t N = W*H;
            auto_ptr<surface> surf( surface::create(fmt, W, H ) );
            
            bytes2read = header.bitsperpixel / 8;
            unit_t n = 0;
#define PXL() do \
{ \
if(n>=N) throw exception("too many read bytes!");\
surf->put_pixel( surf->address_of(n), surf->map_rgba(c) );\
++n; \
} while(false)
            
            
            while(n<N)
            {
                rgb_t c;
                //__________________________________________________________
                //
                // read the color
                //__________________________________________________________
                switch(header.datatypecode)
                {
                    case 1:  /* Indexed uncompressed */
                    {
                        const size_t q = max_of<uint8_t>(header.colourmaplength-1, __fgetb("index"));
                        c = ctable[q+1];
                        PXL();
                    } break;
                        
                        
                    case 2:  /* RGB Uncompressed */
                    case 3:  /* Grey Uncompressed */
                    {
                        if ( int(fread(p,1,bytes2read,fp)) != int(bytes2read))
                            throw libc::exception( EIO, "TGA read RGB");
                        TGA_MergeBytes(&c,p,bytes2read);
                        PXL();
                    } break;
                        
                        
                        
                    case 11: /* Compressed black and white */
                    case 10: /* RGB Compressed */
                    {
                        if (int(fread(p,1,bytes2read+1,fp)) != int(bytes2read+1) )
                            throw libc::exception( EIO, "TGA read Compressed RGB");
                        const size_t j = p[0] & 0x7f;
                        TGA_MergeBytes(&c,&(p[1]),bytes2read);
                        PXL();
                        if(p[0] & 0x80)
                        {
                            /* RLE chunk */
                            for(size_t i=0;i<j;i++)
                            {
                                //TGA_MergeBytes(&c,&(p[1]),bytes2read);
                                PXL();
                            }
                        }
                        else
                        {
                            /* Normal chunk */
                            for (size_t i=0;i<j;i++)
                            {
                                if( int(fread(p,1,bytes2read,fp)) != int(bytes2read) )
                                    throw libc::exception( EIO, "TGA read Compressed RGB");
                                TGA_MergeBytes(&c,p,bytes2read);
                                PXL();
                            }
                        }
                    } break;
                        
                        
                    default:n++;
                }
                
            }
            
            /* Flip the image ? */
            if ((header.imagedescriptor & 0x20) == 32)
            {
                //Flip_Bitmap(image,header.width,header.height,0);
            }
            return surf.yield();
        }
        
    }
}

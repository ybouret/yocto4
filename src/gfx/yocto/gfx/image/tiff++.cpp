#include "yocto/gfx/image/tiff++.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/memory/global.hpp"
#include "tiffio.h"


namespace yocto
{
    namespace gfx
    {
        _TIFF:: _TIFF() throw() : handle(0) {}

        _TIFF:: ~ _TIFF() throw()
        {
            if(handle)
            {
                TIFFClose((TIFF *)handle);
                handle=0;
            }
        }
    }
}


namespace yocto
{
    namespace gfx
    {

        _TIFF:: Raster:: ~Raster() throw()
        {
            cleanup();
        }

        _TIFF:: Raster:: Raster() throw() :
        data(0),
        size(0)
        {
        }

        void _TIFF:: Raster:: cleanup() throw()
        {
            memory::kind<memory::global>::release_as<uint32_t>(data,(size_t&)size);
        }

        void _TIFF:: Raster:: startup(const size_t n)
        {
            if(n>size)
            {
                cleanup();
                (size_t&)size = n;
                data = memory::kind<memory::global>::acquire_as<uint32_t>((size_t&)size);
            }
        }

    }

}

namespace yocto
{
    namespace gfx
    {

        I_TIFF:: I_TIFF(const string &filename) :
        _TIFF()
        {
            handle = TIFFOpen(filename.c_str(),"r");
            if(!handle) throw imported::exception("TIFFOpen","couldn't open '%s'", filename.c_str());
        }

        I_TIFF:: ~I_TIFF() throw()
        {
        }


        bool I_TIFF:: ReadDirectory()
        {
            assert(handle);
            return (1 == TIFFReadDirectory( (TIFF *)handle ));
        }

        int  I_TIFF:: GetWidth()
        {
            uint32_t w = 0;
            if( !TIFFGetField( (TIFF *)handle, TIFFTAG_IMAGEWIDTH, &w ) )
            {
                throw imported::exception("TIFFGetField","invalid width");
            }
            return int(w);
        }

        int  I_TIFF:: GetHeight()
        {
            uint32_t h = 0;
            if( !TIFFGetField( (TIFF *)handle, TIFFTAG_IMAGELENGTH, &h ) )
            {
                throw imported::exception("TIFFGetField","invalid height");
            }
            return int(h);
        }

        void I_TIFF:: SetDirectory(const size_t n)
        {
            if( 1 != TIFFSetDirectory((TIFF *)handle, n) )
            {
                throw imported::exception("TIFFSetDirectory", "couldn't set %u", unsigned(n));
            }
        }

        size_t I_TIFF::CountDirectories()
        {
            size_t count = 0;
            do { ++count; } while( ReadDirectory() );
            return count;
        }

        void I_TIFF:: ReadRBGAImage(Raster &raster)
        {
            const int    w   = GetWidth();
            const int    h   = GetHeight();
            raster.startup(w*h);

            if( 1 != TIFFReadRGBAImage((TIFF*)handle, w, h, raster.data) )
            {
                throw imported::exception("TIFFReadRGBAImage","failure...");
            }
        }


    }
}

#include <cstring>
#include "yocto/memory/buffers.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {

        O_TIFF :: ~O_TIFF() throw()
        {
        }

        O_TIFF:: O_TIFF(const string &filename) :
        _TIFF()
        {
            handle = TIFFOpen(filename.c_str(),"w");
            if(!handle) throw imported::exception("TIFFOpen","couldn't open '%s'", filename.c_str());
            TIFF *out = (TIFF *)handle;
            //TIFFSetField (out, TIFFTAG_IMAGEWIDTH, width);  // set the width of the image
            //TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);    // set the height of the image

            TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);   // set number of channels per pixel
            TIFFSetField(out, TIFFTAG_BITSPERSAMPLE,   8);    // set the size of the channels
            TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    // set the origin of the image.

            //   Some other essential fields to set
            TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(out, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);

            // Initially: no compression
            TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_LZW);

        }

#define O_TIFF_DEFINE(NAME) { #NAME, COMPRESSION_##NAME }
        const O_TIFF::CompressionType
        O_TIFF::NamedCompression[] =
        {
            O_TIFF_DEFINE(NONE),
            O_TIFF_DEFINE(LZW),
            O_TIFF_DEFINE(JPEG),
            O_TIFF_DEFINE(JBIG),
            O_TIFF_DEFINE(PACKBITS),
            O_TIFF_DEFINE(DEFLATE),
            O_TIFF_DEFINE(LZMA),
            {NULL,0}
        };

        void O_TIFF:: SetCompression(const char *cmp)
        {
            unsigned flag = COMPRESSION_NONE;
            if(cmp)
            {
                const CompressionType *ct = NamedCompression;
                while(ct->name)
                {
                    if(0==strcmp(ct->name,cmp))
                    {
                        flag = ct->ttag;
                        break;
                    }
                    ++ct;
                }
            }
            TIFF *out = (TIFF *)handle;
            TIFFSetField(out, TIFFTAG_COMPRESSION, flag);
#if 0
            switch(flag)
            {
                case COMPRESSION_LZW:
                    //TIFFSetField(out, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);
                    break;

                default:
                    ;//TIFFSetField(out, TIFFTAG_PREDICTOR, PREDICTOR_NONE);
            }
#endif

        }

        void O_TIFF:: WriteRGBAImage(const Raster &raster, const int w, const int h)
        {
            assert(w*h<=unit_t(raster.size));
            assert(w>0);
            assert(h>0);
            TIFF *out = (TIFF *)handle;
            TIFFSetField(out, TIFFTAG_IMAGEWIDTH,  w);
            TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);

            const size_t usr_scanline = samples_per_pixel * w;
            const size_t out_scanline = TIFFScanlineSize(out);
            memory::buffer_of<char,memory::global> tmpbuf( max_of(usr_scanline,out_scanline) );
            unsigned char *buf = static_cast<unsigned char *>(tmpbuf.rw());

            // We set the strip size of the file to be size of one row of pixels
            TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out,usr_scanline) );

            //Now writing image to the file one strip at a time
            for(int row=0,top=h; row<h; ++row)
            {
                --top;
                memcpy(buf, &raster.data[top*w], usr_scanline);    // check the index here, and figure out why not using h*linebytes
                if (1 != TIFFWriteScanline(out, buf, row, 0) )
                    break;
            }

            TIFFCheckpointDirectory(out);
            TIFFWriteDirectory(out);
        }
        
        
    }
    
}


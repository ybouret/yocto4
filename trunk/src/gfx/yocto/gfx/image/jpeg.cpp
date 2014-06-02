#include "yocto/gfx/image/jpeg.hpp"
extern "C"
{
#include "yocto/jpeg/jpeglib.h"
}

#include <cstring>
#include "yocto/ios/icstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto
{
    namespace gfx
    {
        jpeg_format:: jpeg_format() : image::format("JPEG")
        {
        }
        
        jpeg_format::~jpeg_format() throw()
        {
            
        }
        
        bool jpeg_format::lossless() const throw()
        {
            return false;
        }
        
        void jpeg_format:: load(const string &filename) const
        {
            static const char fn[] = "jpeg::load";
            YOCTO_GIANT_LOCK();
            
            ios::icstream fp(filename);
            FILE         *fptr = fp.__get();
            struct jpeg_decompress_struct cinfo;
            struct jpeg_error_mgr         jerr;
            memset( &cinfo, 0, sizeof(cinfo) );
            memset( &jerr,  0, sizeof(jerr)  );
            
            //__________________________________________________________________
            //
            // Error handler
            //__________________________________________________________________
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, fptr);
            
            //__________________________________________________________________
            //
            // Read header
            //__________________________________________________________________
            std::cerr << "reading header" << std::endl;
            if( jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK )
            {
                std::cerr << "######## FAILURE" << std::endl;
                throw exception("%s(can't read header)",fn);
            }
            const unit_t width  = cinfo.output_width;
            const unit_t height = cinfo.output_height;
            std::cerr << "w=" << width << ", h=" << height << std::endl;
            
            //__________________________________________________________________
            //
            // buffer for one scan line
            //__________________________________________________________________
            const size_t row_stride = width * cinfo.output_components;
            size_t   buflen = row_stride;
            JSAMPLE *buffer = memory::kind<memory::global>::acquire_as<JSAMPLE>(buflen);
            
            try
            {
                if(!jpeg_start_decompress(&cinfo))
                    throw exception("%s(can't start decompress)",fn);
                
                
                //______________________________________________________________
                //
                // Can only handle RGB JPEG images at this stage
                //______________________________________________________________
                
                if (cinfo.output_components != 3)
                    throw exception("%s(unsupported output_components=%d)", fn, cinfo.output_components);
                
                unit_t j = height-1;
                while (cinfo.output_scanline < cinfo.output_height)
                {
                    if(jpeg_read_scanlines(&cinfo,&buffer,1)!=1)
                    {
                        
                    }
                    for(unit_t i=0;i<cinfo.output_width;i++)
                    {
                        const unit_t i3 = 3*i;
                        const rgba_t C(i3,i3+1,i3+2,255);
                        /*
                         image[j*cinfo.output_width+i].r = buffer[3*i];
                         image[j*cinfo.output_width+i].g = buffer[3*i+1];
                         image[j*cinfo.output_width+i].b = buffer[3*i+2];
                         image[j*cinfo.output_width+i].a = 255;
                         */
                    }
                    j--;
                }
                
                
                // Done
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer,buflen);
                
            }
            catch(...)
            {
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                memory::kind<memory::global>::release_as<JSAMPLE>(buffer,buflen);
                throw;
            }
        }
    }
}
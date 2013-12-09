extern "C" {
#include "yocto/jpeg/jpeglib.h"
}

#include "yocto/gfx/image/jpg.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"

#include "yocto/ptr/auto-arr.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace gfx
    {
        JPG:: JPG() : image::format("jpg")
        {
            
        }
        
        JPG:: ~JPG() throw()
        {
        }
        
        bool JPG:: lossless() const throw()
        {
            return false;
        }
        
        void JPG:: save(const string  &filename,
                        const bitmap  &bmp,
                        addr2rgba     &proc,
						const char    *options) const
        {
            const int width   = bmp.w;
            const int height  = bmp.h;
            int quality       = 75;
            if(options)
            {
                const string opt = options;
                quality = clamp<int>(0,strconv::to<int>(opt,"jpeg quality"),100);
            }
            
            // open the file
            ios::ocstream fp(filename,false);
            
            JSAMPROW row_pointer[1];
            
            // memory, auto released
            auto_arr<char> jimg( 3*size_t(width) );
            JSAMPLE *jimage = (JSAMPLE *)jimg.base();
            
            // Compression Info
            struct jpeg_compress_struct cinfo;
            bzset(cinfo);
            
            // Error handler
            struct jpeg_error_mgr jerr;
            bzset(jerr);;
            cinfo.err = jpeg_std_error(&jerr);
            
            // Initialize JPEG compression object.
            jpeg_create_compress(&cinfo);
            try
            {
                // Associate with output stream
                jpeg_stdio_dest(&cinfo,fp.__get());
                
                // Fill out values
                cinfo.image_width      = width;
                cinfo.image_height     = height;
                cinfo.input_components = 3;
                cinfo.in_color_space   = JCS_RGB;
                
                // Default compression settings
                jpeg_set_defaults(&cinfo);
                jpeg_set_quality(&cinfo, quality, 1); // limit to baseline-JPEG values
                
                // Start compressor
                jpeg_start_compress(&cinfo, 1);
                
                row_pointer[0] = jimage;
                int j = cinfo.image_height - 1;
                const gfx::unit_t top = bmp.h-1;
                while (cinfo.next_scanline < cinfo.image_height)
                {
                    for (int i=0;i<width;i++)
                    {
                        const rgb_t C  = proc( bmp.get(i, top-j) );
                        const int   i3 = 3*i;
                        jimage[i3  ] = C.r;
                        jimage[i3+1] = C.g;
                        jimage[i3+2] = C.b;
                    }
                    jpeg_write_scanlines(&cinfo,row_pointer,1);
                    j--;
                }
                
                
                // End compressor
                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
            }
            catch(...)
            {
                jpeg_finish_compress(&cinfo);
                jpeg_destroy_compress(&cinfo);
                throw;
            }
        }
        
        
        surface * JPG:: load(const string      &filename,
                             const pixel_format fmt) const
        {
            
            ios::icstream fp(filename);
            FILE *fptr = fp.__get();
            struct jpeg_decompress_struct cinfo;
            bzset(cinfo);
            
            struct jpeg_error_mgr jerr;
            bzset(jerr);
            
            // Error handler
            cinfo.err = jpeg_std_error(&jerr);
            
            // Create decompressor
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, fptr);
            
            try
            {
                
                // Read header
                jpeg_read_header(&cinfo, 1);
                jpeg_start_decompress(&cinfo);
                
                const unit_t width  = cinfo.output_width;
                const unit_t height = cinfo.output_height;
                
                // Can only handle RGB JPEG images at this stage
                if (cinfo.output_components != 3)
                    throw imported::exception("jpeg::load", "unsupported #components");
                
                // create surfaces
                auto_ptr<surface> surf( surface::create(fmt, width, height) );
                
                // acquire local memory
                const unit_t input_pitch = cinfo.output_width * cinfo.output_components;
                auto_arr<JSAMPLE> samples( input_pitch );
                JSAMPLE *buffer = samples.base();
                
                
                int j = cinfo.output_height-1;
                while (cinfo.output_scanline < cinfo.output_height)
                {
                    surface::row &row_j = (*surf)[j];
                    (void)jpeg_read_scanlines(&cinfo,&buffer,1);
                    for(unsigned int i=0;i<cinfo.output_width;i++)
                    {
                        const int i3 = 3*i;
                        const rgb_t   c( buffer[i3], buffer[i3+1], buffer[i3+2], 0xff);
                        const pixel_t p = surf->map_rgb(c);;
                        void *addr = row_j[i];
                        surf->put_pixel(addr,p);
                    }
                    j--;
                }
                
                
                // Done
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                
                return surf.yield();
            }
            catch(...)
            {
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                throw;
            }
        }
        
    }
}

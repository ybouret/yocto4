extern "C" {
#include "yocto/jpeg/jpeglib.h"
}

#include "yocto/gfx/image/jpg.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-arr.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/string/conv.hpp"
#include <cstring>

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
            memset(&cinfo,0,sizeof(cinfo));
            
            // Error handler
            struct jpeg_error_mgr jerr;
            memset(&jerr,0,sizeof(jerr));
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
                while (cinfo.next_scanline < cinfo.image_height)
                {
                    for (int i=0;i<width;i++)
                    {
                        const rgb_t C  = proc( bmp.get(i, j) );
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
                jpeg_destroy_compress(&cinfo);
                throw;
            }
        }
        
    }
}

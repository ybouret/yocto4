#include "yocto/gfx/image/png.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/png/png.h"

namespace yocto
{
    namespace gfx
    {
        
        png_format:: ~png_format() throw()
        {
        }
        
        png_format:: png_format() : image::format("PNG")
        {
        }
        
        bool     png_format:: lossless() const throw()
        {
            return false;
        }
        

        
        bitmap  *png_format:: load(const string          &filename,
                                   unit_t                 depth,
                                   image::put_rgba_proc   proc,
                                   const void            *args) const
        {
            static const char fn[] = "png::load";
            //assert(proc);
            YOCTO_GIANT_LOCK();
            
            //__________________________________________________________________
            //
            // open file
            //__________________________________________________________________

            ios::icstream fp(filename);
            
            //__________________________________________________________________
            //
            // read header
            //__________________________________________________________________
            {
                unsigned char header[8];
                size_t        done;
                fp.get(header, 8, done);
                if(done!=8)
                    throw exception("%s(missing header bytes)",fn);
                
                if (png_sig_cmp(header, 0, 8))
                    throw exception("%s(file is not recognized as PNG)", fn);
            }
            
            //__________________________________________________________________
            //
            // prepare to read
            //__________________________________________________________________
            png_structp png_ptr  = 0; png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            png_infop   info_ptr = 0;

            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if(!png_ptr)
                throw exception("couldn't create PNG read struct");
            
            info_ptr = png_create_info_struct(png_ptr);
            if(!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
                throw exception("could't create PNG info struct");
            }
            
            //__________________________________________________________________
            //
            //  Init I/O
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("I/O failure for PNG");
            }
            
            png_init_io(png_ptr, fp.__get());
            png_set_sig_bytes(png_ptr, 8);
            
            png_read_info(png_ptr, info_ptr);
            
            const unit_t     width      = png_get_image_width(png_ptr, info_ptr);
            const unit_t     height     = png_get_image_height(png_ptr, info_ptr);
            const png_byte   color_type = png_get_color_type(png_ptr, info_ptr);
            const png_byte   bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
            
            const int number_of_passes = png_set_interlace_handling(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            std::cerr
            << "width=" << width
            << ", height=" << height
            << ", color_type=" << int(color_type)
            << ", bit_depth=" << int(bit_depth)
            << ", #passes=" << number_of_passes << std::endl;
            
            
            
            
            //__________________________________________________________________
            //
            // cleanup
            //__________________________________________________________________
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }
        
        
        void png_format:: save(const string        &filename,
                               const bitmap        &bmp,
                               image::get_rgba_proc proc,
                               const void          *args,
                               const char          *options) const
        {
        }
        
        
    }
    
}

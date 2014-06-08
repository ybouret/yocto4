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
            
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if(!png_ptr)
                throw exception("couldn't create PNG read struct");
            
            
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

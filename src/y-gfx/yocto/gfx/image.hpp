#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    namespace gfx
    {
        
        struct image
        {
            enum format_t
            {
                TGA   =1,   //!< Targa
                TGA_A =11,  //!< Targa with Alpha
                TGA_Z =12,  //!< Compressed Targa
                TGA_ZA=13,  //!< Compressed Targa with Alpha
                PPM   =2,   //!< PPM
                RGB   =3,   //!< RGB
                BW    =4,   //!< Raw Greyscale
                TIFF  =5,   //!< Tiff
                EPS   =6,   //!< EPS/colour
                EPS_BW=7,   //!< EPS/black and white
                RAW   =8,   //!< Raw
                BMP   =9    //!< BMP
            };
            
            
            /*
             Write a bitmap to a file
             A negative format indicates a vertical flip
             */
            
            
            static void save(ios::ostream &fp,
                             const bitmap &bmp,
                             int           fmt,
                             addr2rgba     &proc);
            
            static void save(const string &filename,
                             const bitmap &bmp,
                             int           fmt,
                             addr2rgba     &proc);
        };
        
    }
}

#endif

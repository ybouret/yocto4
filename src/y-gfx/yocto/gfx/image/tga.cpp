#include "yocto/gfx/image/tga.hpp"
#include "yocto/ios/ocstream.hpp"

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
        
    }
}
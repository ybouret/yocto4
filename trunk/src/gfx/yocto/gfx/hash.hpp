#ifndef YOCTO_GFX_HASH_INCLUDED
#define YOCTO_GFX_HASH_INCLUDED 1

#include "yocto/hashing/function.hpp"
#include "yocto/gfx/bitmap.hpp"

namespace yocto
{
    namespace gfx
    {
        
        void hash_bitmap( const bitmap &bmp, hashing::function &H ) throw();
        
    }
    
}

#endif

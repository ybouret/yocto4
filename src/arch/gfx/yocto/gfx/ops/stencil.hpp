#ifndef YOCTO_GFX_STENCIL_INCLUDED
#define YOCTO_GFX_STENCIL_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/type/args.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <size_t N,typename T>
        class stencil
        {
        public:
            static const size_t SIZE = 1+2*N;
            YOCTO_ARGUMENTS_DECL_T;
    
            mutable_type v[SIZE][SIZE];
            
        };
        
        
    }
}


#endif

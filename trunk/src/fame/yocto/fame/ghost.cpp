#include "yocto/fame/ghost.hpp"

namespace yocto
{
    namespace fame
    {
        
        ghost:: ~ghost() throw() {}
        ghost:: ghost(size_t n) : offsets_list(n) {}
        
        void copy(linear_space       &dst,
                  const ghost        &gdst,
                  const linear_space &src,
                  const ghost        &gsrc) throw()
        {
            assert(gdst.size()==gsrc.size());
            assert(dst.itmsz==src.itmsz);
            
            uint8_t       *target = static_cast<uint8_t *>(dst.data());
            const uint8_t *source = static_cast<const uint8_t *>(src.data());
            const size_t   itmsz  = dst.itmsz;
            const size_t   n      = gdst.size();
            for(size_t i=0;i<n;++i)
            {
                const size_t t = gdst[i];
                const size_t s = gsrc[i];
                memcpy( &target[t*itmsz], &source[s*itmsz], itmsz);
            }
        }
        
    }
    
}


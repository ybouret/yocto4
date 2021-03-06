#include "yocto/fame/ghost.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fame
    {
        
        ghost:: ~ghost() throw() {}
        ghost:: ghost(size_t n) : offsets_list(n) {}
        
        void ghost::copy(linear_space       &dst,
                         const ghost        &gdst,
                         const linear_space &src,
                         const ghost        &gsrc) throw()
        {
            assert(gdst.size()==gsrc.size());
            assert(dst.itmsz==src.itmsz);
            const size_t   n      = gdst.size();
            const size_t   w      = dst.itmsz;
            for(size_t i=n;i>0;--i)
            {
                const size_t t = gdst[i];
                const size_t s = gsrc[i];
                memcpy( dst.data(t), src.data(s), w);
            }
        }
        
        
        
        
        void ghost:: save(void                 *dst,
                          const linear_handles &handles ) const
        {
            
            const size_t n     = size();
            const size_t h     = handles.size();
            //std::cerr << "saving using offsets=" << *this << std::endl;
            const sorted_offsets &self = *this;
            for(size_t j=n;j>0;--j)
            {
                const size_t k = self[j];
                for(size_t i=h;i>0;--i)
                {
                    handles[i].save(dst,k);
                }
                
            }
            
        }
        
        void   ghost:: load(linear_handles &handles, void *src) const
        {
            const size_t n     = size();
            const size_t h     = handles.size();
            
            const sorted_offsets &self = *this;
            for(size_t j=n;j>0;--j)
            {
                const size_t k = self[j];
                for(size_t i=h;i>0;--i)
                {
                    handles[i].load(k,src);
                }
            }
            
        }
        
    }
    
}


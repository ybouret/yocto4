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
            
            const size_t   n      = gdst.size();
            const size_t   w      = dst.itmsz;
            for(size_t i=0;i<n;++i)
            {
                const size_t t = gdst[i];
                const size_t s = gsrc[i];
                memcpy( dst.data(t), src.data(s), w);
            }
        }
        
        
        
        size_t ghost:: chunk_size_of( const array<linear_handle> &handles ) throw()
        {
            size_t ans = 0;
            for(size_t i=handles.size();i>0;--i)
            {
                assert(handles[i]!=NULL);
                ans += handles[i]->itmsz;
            }
            return ans;
        }
        
        size_t ghost:: save(void                       *dst,
                            const size_t                num,
                            const array<linear_handle> &handles ) const throw()
        {

            const size_t n     = size();
            const size_t h     = handles.size();
            
            size_t chunk_size = 0;
            for(size_t i=h;i>0;--i)
            {
                assert(handles[i]!=NULL);
                const linear_space &l = *handles[i];
                chunk_size += l.itmsz;
            }
            assert(chunk_size*n<=num);
            
            const sorted_offsets &self = *this;
            for(size_t j=n;j>0;--j)
            {
                const size_t k = self[j];
                
                
            }
            
            return n*chunk_size;
        }

        
    }
    
}


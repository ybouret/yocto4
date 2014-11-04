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
        
        
        void ghost:: save(array<uint8_t> &dst, const linear_space &src, size_t &offset) const throw()
        {
            const size_t n = size();
            const size_t w = src.itmsz;
            assert(offset>0);
            assert(n*w+offset-1<=dst.size());
            const uint8_t *data   = static_cast<const uint8_t *>(src.data());
            for(size_t i=0;i<n;++i,offset += w)
            {
                const size_t s = (*this)[i];
                memcpy( &dst[offset], &data[s*w], w);
            }
        }
        
        void ghost:: load(linear_space &dst, const array<uint8_t> &src, size_t &offset) const throw()
        {
            const size_t n = size();
            const size_t w = dst.itmsz;
            assert(offset>0);
            assert(n*w+offset-1<=src.size());
            uint8_t *data   = static_cast<uint8_t *>(dst.data());
            for(size_t i=0;i<n;++i,offset += w)
            {
                const size_t d = (*this)[i];
                memcpy( &data[d*w], &src[offset], w);
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
        
        size_t ghost:: save(uint8_t                    *dst,
                            const size_t                num,
                            const array<linear_handle> &handles ) const throw()
        {
            assert(this->size()*chunk_size_of(handles)<=num);

            const size_t n     = size();
            size_t       count = 0;
            
            return count;
        }

        
    }
    
}


#include "yocto/math/ztype.hpp"
#include "yocto/math/sparse/matrix.hpp"

namespace yocto
{
    
    namespace math
    {
        template <>
        spmatrix<z_type>:: ~spmatrix() throw()
        {
            if(size>0)
            {
                assert(na>0);
                ++sa;
                memory::kind<memory::global>::release_as<z_type>(sa,na);
                (size_t&)size = 0;
            }
        }
        
        template <>
        void spmatrix<z_type>::build()
        {
            if(size>0)
            {
                na = size;
                sa = memory::kind<memory::global>::acquire_as<z_type>(na)-1;
            }
            
        }
        
        template <>
        spmatrix<z_type>:: spmatrix(size_t n) :
        size(n),
        sa(0),
        na(0)
        {
            build();
        }
        
        
    }
    
}
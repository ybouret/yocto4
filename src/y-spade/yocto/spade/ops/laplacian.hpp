#ifndef YOCTO_SPADE_OPS_LAPLACIAN_INCLUDED
#define YOCTO_SPADE_OPS_LAPLACIAN_INCLUDED 1

#include "yocto/spade/array3D.hpp"

namespace yocto
{
    namespace spade
    {
        template <typename T>
        struct laplacian
        {
            static inline
            void compute( array1D<T> &L, const array1D<T> &U, const layout1D &zone, const T inv_dx2 ) throw()
            {
                for( unit_t i=zone.lower,im=i-1,ip=i+1;i<=zone.upper;++i,++im,++ip )
                {
                    const T U0 = U[i];
                    const T mid = -(U0+U0);
                    L[i]        = inv_dx2 * ( U[im] + mid + U[ip]);
                }
            }
            
        };
    }
    
}

#endif


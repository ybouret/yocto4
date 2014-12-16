#ifndef YOCTO_MATH_B_SPLINE_INCLUDED
#define YOCTO_MATH_B_SPLINE_INCLUDED 1

#include "yocto/math/v3d.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T, typename VTX>
        VTX BSpline( T x, const array<T> &U, const array<VTX> &V )
        {
            assert(U.size()>=4);
            assert(U.size()==V.size());
            const size_t n = U.size();
            size_t       l = 1;
            if(x<=U[1])
            {
                x = U[1];
            }
            else
            {
                if(x>=U[n])
                {
                    l = n-1;
                    x = U[n];
                }
                else
                {
                    size_t p=n;
                    while(p-l>1)
                    {
                        const size_t m = (p+l)>>1;
                        const T      u = U[m];
                        if(x<u)
                        {
                            p = m;
                        }
                        else
                        {
                            l = m;
                        }
                    }
                }
            }
            
            
            
            return V[l];
        }
        
        
    }
}

#endif


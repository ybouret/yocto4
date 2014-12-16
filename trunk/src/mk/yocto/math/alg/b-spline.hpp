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
            
            size_t       ii[4] = {0};
            if(l<=1)
            {
                ii[0] = 1;
                ii[1] = 2;
                ii[2] = 3;
                ii[3] = 4;
            }
            else
            {
                const size_t lp = l+1;
                if(lp>=n)
                {
                    ii[3] = n;
                    ii[2] = n-1;
                    ii[1] = n-2;
                    ii[0] = n-3;
                }
                else
                {
                    ii[0] = l-1;
                    ii[1] = l;
                    ii[2] = lp;
                    ii[3] = lp+1;
                }
            }
            
            const T   u[4] = { U[ii[0]], U[ii[1]], U[ii[2]], U[ii[3]] };
            const VTX P[4] = { V[ii[0]], V[ii[1]], V[ii[2]], V[ii[3]] };
            
            const T   x4 = u[3] - u[0];
            const VTX D1 = (T(1)/(u[1]-u[0])) * (P[1]-P[0]);
            const VTX D4 = (T(1)/(u[3]-u[2])) * (P[3]-P[2]);
            
            
            return V[l];
        }
        
        
    }
}

#endif


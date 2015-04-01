#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/container/tab2d.hpp"

namespace yocto {
    
    namespace math
    {
        
        inline size_t Cubic_Bspline_Index( const size_t i, const size_t j, const size_t m )
        {
            assert(i<=3);
            assert(j>0);
            assert(j<=m);
            
            if(j<=1)
            {
                // start points
                switch(i)
                {
                    case 0:
                    case 1:
                    case 2:
                        return 1;
                    default:
                        break;
                }
                return 2;
            }
            else
            {
                const size_t mm1 = m-1;
                if(j>=mm1)
                {
                    // End points
                    switch(i)
                    {
                        case 0:
                            return mm1;
                        default:
                            break;
                    }
                    return m;
                }
                else
                {
                    
                }
            }
            return j;
        }
        
        template <typename T,typename U>
        inline U Cubic_Bsplines( const T tt, const array<T> &t, const array<U> &u )
        {
            assert(t.size()==u.size());
            const size_t m = t.size();
            
            if(tt<=t[1])
            {
                return u[1];
            }
            else
            {
                if(tt>=t[m])
                {
                    return u[m];
                }
                else
                {
                    //______________________________________________________
                    //
                    // bracket it
                    //______________________________________________________
                    size_t jlo = 1;
                    size_t jhi = m;
                    
                    while(jhi-jlo>1)
                    {
                        const size_t jmid = (jlo+jhi)>>1;
                        const T      tmid = t[jmid];
                        if(tt>tmid)
                        {
                            jlo = jmid;
                        }
                        else
                        {
                            jhi = jmid;
                        }
                    }
                    
                    const T x  = (tt-t[jlo])/(t[jhi]-t[jlo]);
                    const T x2 = x * x;
                    const T ix = T(1)-x;
                    const T x3 = x2*x;
                    
                    const T b0 = ix*ix*ix/6;
                    const T b1 = (3*x3 - 6*x2 +4)/6;
                    const T b2 = (-3*x3 +3*x2 + 3*x + 1)/6;
                    const T b3 =  x3/6;
                    
                    const U P[4] = {
                        u[Cubic_Bspline_Index(0,jlo,m)],
                        u[Cubic_Bspline_Index(1,jlo,m)],
                        u[Cubic_Bspline_Index(2,jlo,m)],
                        u[Cubic_Bspline_Index(3,jlo,m)] };
                    
                    return b0 * P[0] + b1 * P[1] + b2 * P[2] + b3 * P[3];
                    
                }
            }
            
        }
        
        
    }
}

#endif

#include "yocto/math/ztype.hpp"
#include "yocto/math/alg/hull.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    
    namespace math
    {
        namespace algo2d
        {
            template <>
            void hull<real_t>:: build( sequence<size_t> &h, const array<vertex> &points )
            {
                h.free();
                const size_t n  = points.size();
                if( n <= 0 )
                    return;
                
                if( n == 1)
                {
                    h.push_back(1);
                }
                
                //-- find leftmost point
                size_t indx = 1;
                vertex curr = points[indx];
                for(size_t i=2; i<=n; ++i)
                {
                    const vertex &p = points[i];
                    if( p.x < curr.x)
                    {
                        curr = p;
                        indx = i;
                    }
                }
                
                h.push_back(indx);
                const size_t start = indx;
                do
                {
                    //double theta_min = -1;
                    for(size_t i=n;i>0;--i)
                    {
                        if(i==indx) continue;
                        const vertex &p = points[i];
                        const vertex d(curr,p);
                    }
                    
                }
                while( indx != start );
                
            }
        }
    }
    
}

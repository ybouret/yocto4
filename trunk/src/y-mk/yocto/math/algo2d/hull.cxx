#include "yocto/math/ztype.hpp"
#include "yocto/math/algo2d/hull.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    
    namespace math
    {
        namespace algo2d
        {
            template <>
            void hull<real_t>:: build( sequence<vertex> &h, const array<vertex> &points )
            {
                h.free();
                const size_t n  = points.size();
                if( n <= 0 )
                    return;
                
                
                
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
                
                h.push_back(curr);
                const size_t start = indx;
                double       theta = 0;
                vertex       v( Sin(theta), Cos(theta) );
                do
                {
                    
                }
                while( indx != start );

            }
        }
    }
    
}

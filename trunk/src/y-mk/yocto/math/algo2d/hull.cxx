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
                
                if( n == 1)
                {
                    h.push_back(points[1]);
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
                
                h.push_back(curr);
                const size_t start = indx;
                do
                {
                    for(size_t i=n;i>0;--i)
                    {
                        if(i==indx) continue;
                        const vertex &p = points[i];
                        const vertex v(curr,p);
                        const real_t theta = Atan2(v.x,v.y);
                        std::cerr << "theta=" << theta << std::endl;
                    }
                    
                }
                while( indx != start );

            }
        }
    }
    
}

#ifndef YOCTO_SPADE_GEO_POINT_IN_TRIANGLE2D_INCLUDED
#define YOCTO_SPADE_GEO_POINT_IN_TRIANGLE2D_INCLUDED 1

#include "yocto/spade/types.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace spade
    {

        struct point_in_triangle2d
        {
            template <typename VTX>
            static inline
            bool is_in_bounding_box(const VTX &p,
                                    const VTX &v1,
                                    const VTX &v2,
                                    const VTX &v3,
                                    const typename VTX::type eps) throw()
            {
                const typename VTX::type xmin = min_of(v1.x,min_of(v2.x,v3.x))-eps;
                if(p.x<xmin) return false;
                const typename VTX::type ymin = min_of(v1.y,min_of(v2.y,v3.y))-eps;
                if(p.y<ymin) return false;
                const typename VTX::type xmax = max_of(v1.x,max_of(v2.x,v3.x))+eps;
                if(p.x>xmax) return false;
                const typename VTX::type ymax = max_of(v1.y,max_of(v2.y,v3.y))+eps;
                if(p.y>ymax) return false;
                return true;
            }

            template <typename VTX>
            static inline
            bool naively(const VTX &p,
                         const VTX &v1,
                         const VTX &v2,
                         const VTX &v3) throw()
            {
                const typename VTX::type x1 = v1.x, x2=v2.x, x3=v3.x;
                const typename VTX::type y1 = v1.y, y2=v2.y, y3=v3.y;
                const typename VTX::type dd = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
                if( math::Fabs(dd) <= 0 ) return false;
                const typename VTX::type x=p.x, y=p.y;
                const typename VTX::type a  =  ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3))/dd;
                const typename VTX::type b  =  ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3))/dd;
                const typename VTX::type c  = 1-(a+b);

                 return (0<=a&&a<=1) && (0<=b&&b<=1) && (0<=c&&c<=1);
            }

            template <typename VTX>
            static inline
            typename VTX::type distance_square_to_segment( const VTX &a, const VTX &b, const VTX &p , const typename VTX::type eps_square) throw()
            {
                const VTX ab(a,b);
                const VTX ap(a,p);
                const typename VTX::type abap = ab*ap;
                if(ab*ap<0)
                {
                    return ap*ap;
                }
                else
                {
                    const VTX bp(b,p);
                    const typename VTX::type abbp = ab*bp;
                    if(abbp>0)
                    {
                        return bp*bp;
                    }
                    else
                    {
                        // TODO: finish this...
                        return 0;
                    }
                }


            }

        };

    }
}

#endif

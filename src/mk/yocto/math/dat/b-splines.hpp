#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/container/tab2d.hpp"

namespace yocto {
    
    namespace math
    {
#if 0
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
#endif

        template <typename U>
        struct Cubic_Bsplines
        {
            static inline const U &GetPoint(  const array<U> &Points,  unit_t i ) throw()
            {
                assert(Points.size()>0);
                const unit_t num_points = Points.size();

                if(i<=1)
                {
                    i=1;
                }
                else
                {
                    if(i>=num_points)
                    {
                        i = num_points;
                    }
                    else
                    {

                    }
                }
                return Points[i];

            }

            template <typename T> static inline
            U Get( const T x, const array<U> &Points )
            {
                const unit_t num_points   = Points.size();
                const unit_t num_segments = num_points + 1;
                unit_t start_cv = -2;


                // x: 0 -> 1: find the segment
                const T      xscaled = x*num_segments;
                const unit_t segment = Floor( xscaled );
                const T      t       = (xscaled-segment);
                if(segment>0)
                {
                    //exit(1);
                }
                //std::cerr << "x=" << x << std::endl;
                //std::cerr << "xscaled=" << xscaled << std::endl;

                start_cv += segment;

                const T t2 = t*t;
                const T it = T(1.0)-t;
                const T t3 = t * t2;

                // calculate blending functions for cubic bspline
                const T b0 = it*it*it/T(6.0);
                const T b1 = (3*t3 - 6*t2 + T(4.0))/T(6.0);
                const T b2 = (-3*t3 +3*t2 + 3*t + T(1.0))/T(6.0);
                const T b3 =  t3/T(6.0);

                return
                b0 * GetPoint(Points,start_cv+1) +
                b1 * GetPoint(Points,start_cv+2) +
                b2 * GetPoint(Points,start_cv+3) +
                b3 * GetPoint(Points,start_cv+4);

            }

        };
        
    }
}

#endif
